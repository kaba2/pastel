#ifndef PASTELGEOMETRY_POINT_PATTERN_MATCHING_KR_HPP
#define PASTELGEOMETRY_POINT_PATTERN_MATCHING_KR_HPP

#include "pastel/geometry/point_pattern_matching_kr.h"
#include "pastel/geometry/search_nearest_pointkdtree.h"

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

		template <typename Real, int N, typename Model_PointPolicy, 
			typename Scene_PointPolicy, typename Scene_Model_Output,
			typename NormBijection>
		class PointPatternKr
		{
		public:
			typedef Result_PointPatternMatchKr<Real, N> Match;

			typedef PointKdTree<Real, N, Model_PointPolicy> ModelTree;
			typedef typename ModelTree::Point_ConstIterator 
				Model_ConstIterator;
			typedef typename ModelTree::Point ModelPoint;

			typedef PointKdTree<Real, N, Scene_PointPolicy> SceneTree;
			typedef typename SceneTree::Point_ConstIterator 
				Scene_ConstIterator;
			typedef typename SceneTree::Point ScenePoint;

			typedef std::pair<Scene_ConstIterator, Model_ConstIterator> Pair;
			typedef std::vector<Pair> PairSet;
			typedef typename PairSet::iterator Pair_Iterator;
			typedef typename PairSet::const_iterator Pair_ConstIterator;

			Match match(
				const PointKdTree<Real, N, Model_PointPolicy>& modelTree,
				const PointKdTree<Real, N, Scene_PointPolicy>& sceneTree,
				integer kNearest,
				const Real& minMatchRatio,
				const Real& actualMatchingDistance,
				const Real& maxBias,
				MatchingMode::Enum matchingMode,
				const NormBijection& normBijection,
				const Scene_Model_Output& report)
			{
				/*
				This function implements my algorithm from an upcoming paper
				'Quantitative analysis of dynamic association in live biological 
				fluorescent samples'.
				*/

				const integer d = modelTree.n();

				std::vector<Model_ConstIterator> modelSet(
					countingIterator(modelTree.begin()),
					countingIterator(modelTree.end()));
				std::random_shuffle(modelSet.begin(), modelSet.end());

				const Real matchingDistance = 
					normBijection.toBijection(actualMatchingDistance);

				Vector<Real, N> searchPoint(ofDimension(d));
				Vector<Real, N> translation(ofDimension(d));
				// We allow relative error for the nearest neighbor,
				// since this is an approximate algorithm anyway.
				// This gives us some performance boost.
				const Real maxRelativeError = 1;

				const integer minMatches = 
					std::min((integer)std::ceil(minMatchRatio * modelSet.size()),
					(integer)modelSet.size());

				PairSet bestPairSet;
				Vector<Real, N> bestTranslation(ofDimension(d));
				Real bestBias = 1;

				bool exitEarly = false;

				const integer n = modelSet.size();
				for (integer i = 0;i < n && !exitEarly;++i)
				{
					// Pick a model pivot point.
					const Model_ConstIterator modelPivotIter = modelSet[i];

					// Go over all scene pivot points.
					Scene_ConstIterator scenePivotIter = sceneTree.begin();
					const Scene_ConstIterator scenePivotEnd = sceneTree.end();
					while(scenePivotIter != scenePivotEnd && !exitEarly)
					{
						translation = 
							sceneTree.pointPolicy()(
							scenePivotIter->point()) -
							modelTree.pointPolicy()(
							modelPivotIter->point());

						// Find out how many points match
						// under this translation.

						PairSet candidatePairSet;
						for (integer j = 0;j < modelSet.size();++j)
						{
							const Model_ConstIterator modelIter = modelSet[j];
							
							searchPoint = modelTree.pointPolicy()(
								modelIter->point()) + 
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
								neighborOutput,	All_Indicator(),
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
								const Pair_ConstIterator iterEnd = pairSet.end();
								while(iter != iterEnd)
								{
									meanDelta += 
										sceneTree.pointPolicy()(iter->first->point()) -
										(modelTree.pointPolicy()(iter->second->point()) + 
										translation);

									++iter;
								}

								meanDelta /= pairSet.size();

								const Real meanNorm = 
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

	template <typename Real, int N, typename Model_PointPolicy, 
		typename Scene_PointPolicy, typename Scene_Model_Output,
		typename NormBijection>
	Result_PointPatternMatchKr<Real, N> pointPatternMatchKr(
		const PointKdTree<Real, N, Model_PointPolicy>& modelTree,
		const PointKdTree<Real, N, Scene_PointPolicy>& sceneTree,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& matchingDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxBias,
		MatchingMode::Enum matchingMode,
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
			Real, N, Model_PointPolicy, 
			Scene_PointPolicy, Scene_Model_Output,
			NormBijection> 
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
