#ifndef PASTELGEOMETRY_MATCH_POINTS_KR_HPP
#define PASTELGEOMETRY_MATCH_POINTS_KR_HPP

#include "pastel/geometry/pattern_matching/match_points_kr.h"
#include "pastel/geometry/search_nearest_kdtree.h"

#include "pastel/sys/iterator/counting_iterator.h"
#include "pastel/sys/graph/maximum_bipartite_matching.h"
#include "pastel/sys/output.h"
#include "pastel/sys/range.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/stdpair_as_pair.h"
#include "pastel/sys/hashing/iteratoraddress_hash.h"

#include <boost/range/adaptor/transformed.hpp>
#include <map>
#include <vector>

namespace Pastel
{

	template <
		typename Model_Settings, template <typename> class Model_Customization,
		typename Scene_Settings, template <typename> class Scene_Customization,
		typename Locator = typename Scene_Settings::Locator,
		typename Real = typename Locator::Real,
		integer N = Locator::N,
		typename... ArgumentSet>
	auto matchPointsKr(
		const PointKdTree<Model_Settings, Model_Customization>& modelTree,
		const PointKdTree<Scene_Settings, Scene_Customization>& sceneTree,
		ArgumentSet&&... argumentSet)
		-> Result_MatchPointsKr<Real, N>;
	{
		integer kNearest = 
			PASTEL_ARG_S(kNearest, 16);
		Real minMatchRatio = 
			PASTEL_ARG_S(minMatchRatio, 0.8);
		Real matchingDistance = 
			PASTEL_ARG_S(matchingDistance, 0.1);
		Real maxBias = 
			PASTEL_ARG_S(maxBias, 0.1);
		MatchingMode matchingMode = 
			PASTEL_ARG_S(mathingMode, MatchingMode::FirstMatch);
		auto&& normBijection = PASTEL_ARG(
			normBijection,
			[](){return Euclidean_NormBijection<Real>();},
			[](auto input) {return implicitArgument(Models<decltype(input), NormBijection_Concept>());});
		auto&& report = PASTEL_ARG(
			report,
			[](){return nullOutput();},
			[](auto input) {return std::true_type();});

		ENSURE_OP(kNearest, >, 0);
		ENSURE(minMatchRatio >= 0);
		ENSURE(minMatchRatio <= 1);
		ENSURE(matchingDistance >= 0);
		ENSURE(maxBias >= 0);
		ENSURE(maxBias <= 1);

		using Match = Result_MatchPointsKr<Real, N>;

		using ModelTree = PointKdTree<Model_Settings, Model_Customization>;
		using Model_ConstIterator = typename ModelTree::Point_ConstIterator;
		using ModelPoint = typename ModelTree::Point;

		using SceneTree = PointKdTree<Scene_Settings, Scene_Customization>;
		using Scene_ConstIterator = typename SceneTree::Point_ConstIterator;
		using ScenePoint = typename SceneTree::Point;

		using Pair = std::pair<Scene_ConstIterator, Model_ConstIterator>;
		using PairSet = std::vector<Pair>;
		using Pair_Iterator = typename PairSet::iterator;
		using Pair_ConstIterator = typename PairSet::const_iterator;

		/*
		'Quantitative analysis of dynamic association in 
		live biological fluorescent samples',
		Pekka Ruusuvuori, Lassi Paavolainen, Kalle Rutanen, 
		Anita Mäki, Heikki Huttunen, Varpu Marjomäki, 
		PLoS ONE 9 (4), 2014.
		*/

		integer d = modelTree.n();

		std::vector<Model_ConstIterator> modelSet(
			countingIterator(modelTree.begin()),
			countingIterator(modelTree.end()));
		std::random_shuffle(modelSet.begin(), modelSet.end());

		Real matchingDistance = 
			normBijection.toBijection(actualMatchingDistance);

		Vector<Real, N> searchPoint(ofDimension(d));
		Vector<Real, N> translation(ofDimension(d));
		// We allow relative error for the nearest neighbor,
		// since this is an approximate algorithm anyway.
		// This gives us some performance boost.
		Real maxRelativeError = 1;

		integer minMatches =
			std::min(
				(integer)ceil(minMatchRatio * (integer)modelSet.size()),
				(integer)modelSet.size()
			);

		PairSet bestPairSet;
		Vector<Real, N> bestTranslation(ofDimension(d));
		Real bestBias = 1;

		bool exitEarly = false;

		integer n = modelSet.size();
		for (integer i = 0;i < n && !exitEarly;++i)
		{
			// Pick a model pivot point.
			Model_ConstIterator modelPivotIter = modelSet[i];

			// Go over all scene pivot points.
			Scene_ConstIterator scenePivotIter = sceneTree.begin();
			Scene_ConstIterator scenePivotEnd = sceneTree.end();

			while(scenePivotIter != scenePivotEnd && !exitEarly)
			{
				translation =
					pointAsVector(location(scenePivotIter->point(), sceneTree.locator())) -
					pointAsVector(location(modelPivotIter->point(), modelTree.locator()));

				// Find out how many points match
				// under this translation.

				PairSet candidatePairSet;
				for (integer j = 0;j < modelSet.size();++j)
				{
					Model_ConstIterator modelIter = modelSet[j];
					
					searchPoint = pointAsVector(location(modelIter->point(), modelTree.locator())) + 
						translation;


					auto neighborOutput = [&](
						const Real& distance,
						const Scene_ConstIterator& scene)
					{
						candidatePairSet.push_back(
							std::make_pair(scene, modelIter));
					};

					searchNearest(
						sceneTree, 
						searchPoint,
						PASTEL_TAG(nearestOutput), neighborOutput,
						PASTEL_TAG(acceptPoint), allIndicator(),
						PASTEL_TAG(normBijection), normBijection,
						PASTEL_TAG(k), kNearest,
						PASTEL_TAG(maxDistance), matchingDistance,
						PASTEL_TAG(maxRelativeError), maxRelativeError);

					//log() << "Distance " << neighbor.key() << logNewLine;
				
				}

				using namespace boost::adaptors;

				// FIX: It's a bit inefficient to use polymorphic function
				// objects. But the Boost::Range adaptor can not currently
				// deal with lambdas directly.
				std::function<Scene_ConstIterator(const Pair&)> firstElement =
					[](const Pair& pair) {return pair.first;};

				std::function<Model_ConstIterator(const Pair&)> secondElement =
					[](const Pair& pair) {return pair.second;};

				PairSet pairSet;
				maximumBipartiteMatching(
					candidatePairSet | transformed(firstElement),
					candidatePairSet | transformed(secondElement),
					pushBackOutput(pairSet),
					IteratorAddress_Hash(),
					IteratorAddress_Hash());

				//log() << pairSet.size() << " ";

				if (!pairSet.empty() &&
					pairSet.size() >= minMatches &&
					pairSet.size() >= bestPairSet.size())
				{
					// We have found a possible match.

					// Compute the bias of the match.
					// The bias is the norm of the mean difference
					// between the pairs, divided by the matching distance.
					Real bias = 0;
					{
						Vector<Real, N> meanDelta(ofDimension(d));

						Pair_ConstIterator iter = pairSet.begin();
						Pair_ConstIterator iterEnd = pairSet.end();
						while(iter != iterEnd)
						{
							meanDelta += 
								pointAsVector(location(iter->first->point(), sceneTree.locator())) -
								(pointAsVector(location(iter->second->point(), modelTree.locator())) + 
								translation);

							++iter;
						}

						meanDelta /= pairSet.size();

						Real meanNorm = 
							normBijection.toNorm(norm2(meanDelta, normBijection));

						bias = meanNorm / actualMatchingDistance;
					}							

					// Check that the bias is not too large.
					if (bias <= maxBias)
					{
						// We have a match.

						// See if it is better than the existing
						// match. Larger match size is primarily better,
						// smaller bias is secondarily better.
						if (pairSet.size() > bestPairSet.size() ||

							(pairSet.size() == bestPairSet.size() &&
							bias < bestBias))
						{
							bestPairSet.swap(pairSet);
							bestTranslation = translation;
							bestBias = bias;
						}

						if (matchingMode == MatchingMode::FirstMatch)
						{
							// The first match was asked for,
							// so return this match.
							exitEarly = true;
						}
						// else we are looking for the best match,
						// and continue searching.
					}
				}

				++scenePivotIter;
			}
		}

		bool success = false;
		if (!bestPairSet.empty())
		{
			// A match was found. Report it.
			success = true;

			std::for_each(
				bestPairSet.begin(), bestPairSet.end(),
				[&](const std::pair<Scene_ConstIterator, Model_ConstIterator>& that)
			{
				report(that);
			});
		}

		return Match{ success, bestTranslation, bestBias };
	}

}

#endif
