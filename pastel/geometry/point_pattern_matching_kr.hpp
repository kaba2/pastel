#ifndef PASTELGEOMETRY_POINT_PATTERN_MATCHING_KR_HPP
#define PASTELGEOMETRY_POINT_PATTERN_MATCHING_KR_HPP

#include "pastel/geometry/point_pattern_matching_kr.h"
#include "pastel/geometry/pointkdtree/pointkdtree_search_nearest.h"

#include "pastel/sys/counting_iterator.h"
#include "pastel/sys/maximum_bipartite_matching.h"
#include "pastel/sys/outputs.h"
#include "pastel/sys/range.h"
#include "pastel/sys/stdpair_as_pair.h"
#include "pastel/sys/iteratoraddress_hash.h"

#include <boost/range/adaptor/transformed.hpp>
#include <map>

namespace Pastel
{

	namespace PointPatternMatch_
	{

		template <
			typename Scene_Settings, template <typename> class Scene_Customization,
			typename Model_Settings, template <typename> class Model_Customization,
			typename Scene_Model_Output,
			typename NormBijection,
			typename Locator>
		class PointPatternKr
		{
		public:
			using Real = typename Locator::Real;
			static PASTEL_CONSTEXPR integer N = Locator::N;

			using Match = Result_PointPatternMatchKr<Real, N>;

			using ModelTree = PointKdTree<Model_Settings, Model_Customization>;
			typedef typename ModelTree::Point_ConstIterator 
				Model_ConstIterator;
			using ModelPoint = typename ModelTree::Point;

			using SceneTree = PointKdTree<Scene_Settings, Scene_Customization>;
			typedef typename SceneTree::Point_ConstIterator 
				Scene_ConstIterator;
			using ScenePoint = typename SceneTree::Point;

			using Pair = std::pair<Scene_ConstIterator, Model_ConstIterator>;
			using PairSet = std::vector<Pair>;
			using Pair_Iterator = typename PairSet::iterator;
			using Pair_ConstIterator = typename PairSet::const_iterator;

			Match match(
				const ModelTree& modelTree,
				const SceneTree& sceneTree,
				integer kNearest,
				const Real& minMatchRatio,
				const Real& actualMatchingDistance,
				const Real& maxBias,
				MatchingMode matchingMode,
				const NormBijection& normBijection,
				const Scene_Model_Output& report)
			{
				/*
				This function implements my algorithm from an upcoming paper
				'Quantitative analysis of dynamic association in live biological 
				fluorescent samples'.
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
					std::min((integer)std::ceil(minMatchRatio * modelSet.size()),
					(integer)modelSet.size());

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
							pointAsVector(scenePivotIter->point(), sceneTree.locator()) -
							pointAsVector(modelPivotIter->point(), modelTree.locator());

						// Find out how many points match
						// under this translation.

						PairSet candidatePairSet;
						for (integer j = 0;j < modelSet.size();++j)
						{
							Model_ConstIterator modelIter = modelSet[j];
							
							searchPoint = pointAsVector(modelIter->point(), modelTree.locator()) + 
								translation;


							auto neighborOutput = [&](
								const Real& distance,
								const Scene_ConstIterator& scene)
							{
								candidatePairSet.push_back(
									std::make_pair(scene, modelIter));
							};

							searchNearest(
								sceneTree, searchPoint,
								neighborOutput,	allIndicator(),
								normBijection)
								.kNearest(kNearest)
								.maxDistance(matchingDistance)
								.maxRelativeError(maxRelativeError);

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
										pointAsVector(iter->first->point(), sceneTree.locator()) -
										(pointAsVector(iter->second->point(), modelTree.locator()) + 
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

				// TODO: Replace with uniform initialization later.
				return Match(success, bestTranslation, bestBias);
			}
		};
	
	}

	template <
		typename Model_Settings, template <typename> class Model_Customization,
		typename Scene_Settings, template <typename> class Scene_Customization,
		typename NormBijection,
		typename Scene_Model_Output,
		typename Locator,
		typename Real,
		integer N>
	Result_PointPatternMatchKr<Real, N> pointPatternMatchKr(
		const PointKdTree<Model_Settings, Model_Customization>& modelTree,
		const PointKdTree<Scene_Settings, Scene_Customization>& sceneTree,
		integer kNearest,
		const NoDeduction<Real>& minMatchRatio,
		const NoDeduction<Real>& matchingDistance,
		const NoDeduction<Real>& maxBias,
		MatchingMode matchingMode,
		const NormBijection& normBijection,
		Scene_Model_Output report)
	{
		ENSURE_OP(kNearest, >, 0);
		ENSURE_OP(minMatchRatio, >=, 0);
		ENSURE_OP(minMatchRatio, <=, 1);
		ENSURE_OP(matchingDistance, >=, 0);
		ENSURE_OP(maxBias, >=, 0);
		ENSURE_OP(maxBias, <=, 1);

		PointPatternMatch_::PointPatternKr<
			Model_Settings, Model_Customization,
			Scene_Settings, Scene_Customization,
			Scene_Model_Output,
			NormBijection, Locator> 
			pointPattern;

		return pointPattern.match(
			modelTree, sceneTree,
			kNearest,
			minMatchRatio,
			matchingDistance,
			maxBias,
			matchingMode,
			normBijection,
			report);
	}

}

#endif
