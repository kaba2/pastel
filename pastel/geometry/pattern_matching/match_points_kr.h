// Description: Point pattern matching under translations
// Documentation: match_points_kr.txt

#ifndef PASTELGEOMETRY_MATCH_POINTS_KR_H
#define PASTELGEOMETRY_MATCH_POINTS_KR_H

#include "pastel/geometry/nearestset/nearestset_concept.h"

#include "pastel/geometry/pointkdtree/pointkdtree.h"

// Optional argument requirements

#include "pastel/sys/output/output_concept.h"
#include "pastel/math/normbijection/normbijection_concept.h"

// Default optional arguments

#include "pastel/sys/output/null_output.h"
#include "pastel/math/normbijection/euclidean_normbijection.h"

// Implementation

#include "pastel/sys/iterator/counting_iterator.h"
#include "pastel/sys/hashing/iteratoraddress_hash.h"
#include "pastel/sys/graph/matching/maximum_bipartite_matching.h"
#include "pastel/sys/range.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/output/push_back_output.h"
#include "pastel/sys/stdpair_as_pair.h"
#include "pastel/sys/hashing/iteratoraddress_hash.h"

#include "pastel/geometry/search_nearest_kdtree.h"

#include <vector>
#include <unordered_map>

namespace Pastel
{

	enum class MatchPointsKr_Mode : integer
	{
		FirstMatch,
		MaximumMatch
	};

	template <typename Real, integer N>
	struct MatchPointsKr_Return
	{
		bool success;
		Vector<Real, N> translation;
		Real bias;
	};

	//! Find a matching translation between unpaired point-sets.
	/*!
	Preconditions:
	kNearest > 0
	minMatchRatio >= 0
	maxBias >= 0
	maxBias <= 1
	matchingDistance2 >= 0

	model:
	The point-set to search for.

	scene:
	The point-set from which to search for.

	Optional arguments
	------------------

	kNearest (integer : 16):
	The number of nearest neighbors to use for disambiguation
	in case a point has multiple candidate pairs in its 
	matching distance.

	matchingMode (MatchPointsKr_Mode : FirstMatch):
	MatchPointsKr_Mode::FirstMatch: Accept the first match.
	MatchPointsKr_Mode::BestMatch: Search for the best match.

	matchingDistance2 (Real : 0.1):
	The maximum distance between a point in the model-set
	and a point in the scene-set to accept them as a pair.
	Measured in terms of the norm-bijection.

	maxBias (real : 0.1):
	The maximum bias which to accept from a match.
	The bias is the norm-bijection of the mean
	error, divided by 'matchingDistance2'.
	Matches with larger bias will be ignored.

	minMatchRatio (Real : 0.8):
	The minimum ratio of model-points that need to be 
	paired scene-points to accept a match.

	normBijection (NormBijection : Euclidean_NormBijection<Real>()):
	The distance measure.

	report (Output(Point, Point) : nullOutput()):
	The point-pairs are reported in the form
	std::make_pair(modelPoint, sceneIter).

	Returns
	-------

	success (bool):
	Whether a match was found.

	translation (Vector<Real, N>):
	The translation which maps model-set to the scene-set.

	bias (Real):
	The bias of the match; in the range [0, 1].
	*/
	template <
		typename Model_NearestSet,
		typename Scene_NearestSet,
		typename Locator = PointSet_Locator<NearestSet_PointSet<Model_NearestSet>>,
		typename Real = NearestSet_Real<Model_NearestSet>,
		integer N = Locator::N,
		typename... ArgumentSet,
		Requires<
			Models<Model_NearestSet, NearestSet_Concept>,
			Models<Scene_NearestSet, NearestSet_Concept>
		> = 0>
	auto matchPointsKr(
		const Model_NearestSet& model,
		const Scene_NearestSet& scene,
		ArgumentSet&&... argumentSet)
		-> MatchPointsKr_Return<Real, N>
	{
		integer kNearest = 
			PASTEL_ARG_S(kNearest, 16);
		// This is deliberately real (not Real).
		real minMatchRatio = 
			PASTEL_ARG_S(minMatchRatio, 0.8);
		Real matchingDistance2 = 
			PASTEL_ARG_S(matchingDistance2, 0.1);
		Real maxBias = 
			PASTEL_ARG_S(maxBias, 0.1);
		MatchPointsKr_Mode matchingMode = 
			PASTEL_ARG_S(matchingMode, MatchPointsKr_Mode::FirstMatch);
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
		ENSURE(matchingDistance2 >= 0);
		ENSURE(maxBias >= 0);
		ENSURE(maxBias <= 1);

		using Match = MatchPointsKr_Return<Real, N>;

		using Model_ConstIterator = typename Model_NearestSet::Point_ConstIterator;
		using ModelPoint = typename Model_NearestSet::Point;

		using Scene_ConstIterator = typename Scene_NearestSet::Point_ConstIterator;
		using ScenePoint = typename Scene_NearestSet::Point;

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

		integer d = model.n();

		std::vector<Model_ConstIterator> indexToModel(
			countingIterator(model.begin()),
			countingIterator(model.end()));
		std::random_shuffle(indexToModel.begin(), indexToModel.end());

		std::vector<Scene_ConstIterator> indexToScene(
			countingIterator(scene.begin()),
			countingIterator(scene.end()));

		std::unordered_map<Scene_ConstIterator, integer, IteratorAddress_Hash> sceneToIndex;
		{
			integer j = 0;
			for (auto i = scene.begin();i != scene.end();++i)
			{
				sceneToIndex[i] = j;
				++j;
			}
		}

		Vector<Real, N> searchPoint(ofDimension(d));
		Vector<Real, N> translation(ofDimension(d));
		// We allow relative error for the nearest neighbor,
		// since this is an approximate algorithm anyway.
		// This gives us some performance boost.
		Real maxRelativeError = 1;

		integer minMatches =
			std::min(
				(integer)ceil(minMatchRatio * (integer)indexToModel.size()),
				(integer)indexToModel.size()
			);

		PairSet bestPairSet;
		Vector<Real, N> bestTranslation(ofDimension(d));
		Real bestBias = 1;

		Array<integer> nearestSet(Vector2i(indexToModel.size(), kNearest));

		bool exitEarly = false;

		integer n = indexToModel.size();
		for (integer i = 0;i < n && !exitEarly;++i)
		{
			// Pick a model pivot point.
			Model_ConstIterator modelPivot = indexToModel[i];

			// Go over all scene pivot points.
			Scene_ConstIterator scenePivot = scene.begin();
			Scene_ConstIterator scenePivotEnd = scene.end();

			while(scenePivot != scenePivotEnd && !exitEarly)
			{
				translation =
					pointAsVector(location(scenePivot->point(), scene.locator())) -
					pointAsVector(location(modelPivot->point(), model.locator()));

				// Find out how many points match
				// under this translation.
				std::fill(nearestSet.begin(), nearestSet.end(), -1);
				for (integer j = 0;j < indexToModel.size();++j)
				{
					Model_ConstIterator modelPoint = indexToModel[j];
					
					searchPoint = 
						pointAsVector(
							location(modelPoint->point(), model.locator())
						) + translation;

					integer k = 0;
					auto neighborOutput = [&](
						const Real& distance,
						const Scene_ConstIterator& scenePoint)
					{
						nearestSet(j, k) = sceneToIndex[scenePoint];
						++k;
					};

					searchNearest(
						scene, 
						searchPoint,
						normBijection,
						PASTEL_TAG(nearestOutput), neighborOutput,
						PASTEL_TAG(kNearest), kNearest,
						PASTEL_TAG(maxDistance2), matchingDistance2,
						PASTEL_TAG(maxRelativeError), maxRelativeError);

					//log() << "Distance " << neighbor.key() << logNewLine;
				
				}

				auto forEachAdjacent = [&](integer iModel, auto&& visit)
				{
					for (integer k = 0;k < kNearest;++k)
					{
						integer iScene = nearestSet(iModel, k);
						if (iScene < 0 || !visit(iScene))
						{
							break;
						}
					}
				};

				PairSet pairSet;
				maximumBipartiteMatching(
					indexToModel.size(),
					sceneToIndex.size(),
					forEachAdjacent,
					PASTEL_TAG(report), 
					[&](integer iModel, integer iScene)
					{
						pairSet.push_back(std::make_pair(indexToScene[iScene], indexToModel[iModel]));
					});

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
								pointAsVector(location(iter->first->point(), scene.locator())) -
								(pointAsVector(location(iter->second->point(), model.locator())) + 
								translation);

							++iter;
						}

						meanDelta /= pairSet.size();

						Real meanDeltaNorm = 
							norm2(meanDelta, normBijection);

						bias = meanDeltaNorm / matchingDistance2;
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

						if (matchingMode == MatchPointsKr_Mode::FirstMatch)
						{
							// The first match was asked for,
							// so return this match.
							exitEarly = true;
						}
						// else we are looking for the best match,
						// and continue searching.
					}
				}

				++scenePivot;
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
