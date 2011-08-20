#ifndef PASTEL_POINT_PATTERN_MATCH_GMO_HPP
#define PASTEL_POINT_PATTERN_MATCH_GMO_HPP

#include "pastel/geometry/point_pattern_match_gmo.h"
#include "pastel/geometry/search_nearest_one_pointkdtree.h"

#include "pastel/sys/countingiterator.h"
#include "pastel/sys/maximum_bipartite_matching.h"
#include "pastel/sys/reporters.h"
#include "pastel/sys/iterator_range.h"
#include "pastel/sys/stdpair_as_pair.h"

#include <map>

namespace Pastel
{

	namespace Detail_PointPatternMatch
	{

		template <typename Real, int N, typename Model_PointPolicy, 
			typename Scene_PointPolicy, typename SceneModel_Iterator,
			typename NormBijection>
		class PointPatternGmo
		{
		public:
			typedef PointKdTree<Real, N, Model_PointPolicy> ModelTree;
			typedef typename ModelTree::Point_ConstIterator 
				Model_ConstIterator;
			typedef typename ModelTree::Point ModelPoint;

			typedef PointKdTree<Real, N, Scene_PointPolicy> SceneTree;
			typedef typename SceneTree::Point_ConstIterator 
				Scene_ConstIterator;
			typedef typename SceneTree::Point ScenePoint;

			typedef std::vector<std::pair<Scene_ConstIterator, Model_ConstIterator> > 
				PairSet;
			typedef typename PairSet::iterator Pair_Iterator;
			typedef typename PairSet::const_iterator Pair_ConstIterator;

			class Neighbor_Reporter
			{
			public:
				explicit Neighbor_Reporter(
					PairSet& pairSet,
					const Model_ConstIterator& model)
					: pairSet_(pairSet)
					, model_(model)
				{
				}

				void operator()(const Scene_ConstIterator& scene) const
				{
					pairSet_.push_back(
						std::make_pair(scene, model_));
				}

			private:
				PairSet& pairSet_;
				Model_ConstIterator model_;
			};

			bool match(
				const PointKdTree<Real, N, Model_PointPolicy>& modelTree,
				const PointKdTree<Real, N, Scene_PointPolicy>& sceneTree,
				const Real& minMatchRatio,
				const Real& actualMatchingDistance,
				MatchingMode::Enum matchingMode,
				const NormBijection& normBijection,
				Vector<Real, N>& outTranslation,
				Real& outStability,
				SceneModel_Iterator output)
			{
				const integer d = modelTree.dimension();

				outTranslation = Vector<Real, N>(ofDimension(d), 0);
				outStability = 0;

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
				Real bestStability = 0;

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

							const integer kNearest = 16;

							searchNearest(
								sceneTree, 
								searchPoint,
								kNearest,
								Neighbor_Reporter(candidatePairSet, modelIter),
								nullReporter(),
								matchingDistance,
								maxRelativeError,
								Always_AcceptPoint<Scene_ConstIterator>(),
								16,
								normBijection);

							//log() << "Distance " << neighbor.key() << logNewLine;
						
						}

						PairSet pairSet;
						maximumBipartiteMatching(
							range(candidatePairSet.begin(), candidatePairSet.end()),
							pushBackReporter(pairSet),
							StdPair_As_Pair(),
							iteratorAddressHash(sceneTree.end()),
							iteratorAddressHash(modelTree.end()));

						//log() << pairSet.size() << " ";

						if (pairSet.size() >= minMatches &&
							pairSet.size() >= bestPairSet.size())
						{
							// We have found a match.

							// Compute the stability of the match.
							// The stability is the minimum percentage of the
							// matching distance such that a outTranslation by
							// that distance would lose the match.
							Real stability = 0;
							{
								Real maxDistance = 0;

								Pair_ConstIterator iter = pairSet.begin();
								const Pair_ConstIterator iterEnd = pairSet.end();
								while(iter != iterEnd)
								{
									const Real distance = 
										norm2(
										sceneTree.pointPolicy()(iter->first->point()) -
										(modelTree.pointPolicy()(iter->second->point()) + translation),
										normBijection);

									if (distance > maxDistance)
									{
										maxDistance = distance;
									}

									++iter;
								}

								stability = 
									1 - normBijection.toNorm(maxDistance) /
									actualMatchingDistance;
							}							

							if (matchingMode == MatchingMode::FirstMatch)
							{
								// The first match was asked for,
								// so return this match.

								bestPairSet.swap(pairSet);
								bestTranslation = translation;
								bestStability = stability;

								// No need to look for additional matches.
								exitEarly = true;
							}
							else if (matchingMode == MatchingMode::MaximumMatch)
							{
								// A maximum match was asked for,
								// so the greater-than here is on purpose
								// (there can be many maximum matches).
								// Between matches of the same size,
								// we pick the more stable one.

								if (pairSet.size() > bestPairSet.size() ||
									(pairSet.size() == bestPairSet.size() &&
									stability > bestStability))
								{
									bestPairSet.swap(pairSet);
									bestTranslation = translation;
									bestStability = stability;
								}
							}
						}

						++scenePivotIter;
					}
				}

				if (!bestPairSet.empty())
				{
					// A match was found. Report it.

					std::copy(
						bestPairSet.begin(), bestPairSet.end(),
						output);
					outTranslation = bestTranslation;
					outStability = bestStability;

					return true;
				}

				return false;
			}
		};
	
	}

	template <typename Real, int N, typename Model_PointPolicy, 
		typename Scene_PointPolicy, typename SceneModel_Iterator,
		typename NormBijection>
	bool pointPatternMatchGmo(
		const PointKdTree<Real, N, Model_PointPolicy>& modelTree,
		const PointKdTree<Real, N, Scene_PointPolicy>& sceneTree,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& matchingDistance,
		MatchingMode::Enum matchingMode,
		const NormBijection& normBijection,
		Vector<Real, N>& outTranslation,
		PASTEL_NO_DEDUCTION(Real)& outStability,
		SceneModel_Iterator output)
	{
		ENSURE_OP(minMatchRatio, >=, 0);
		ENSURE_OP(minMatchRatio, <=, 1);
		ENSURE_OP(matchingDistance, >=, 0);

		Detail_PointPatternMatch::PointPatternGmo<
			Real, N, Model_PointPolicy, 
			Scene_PointPolicy, SceneModel_Iterator,
			NormBijection> 
			pointPattern;

		return pointPattern.match(
			modelTree, sceneTree, 
			minMatchRatio,
			matchingDistance,
			matchingMode,
			normBijection,
			outTranslation, 
			outStability,
			output);
	}

}

#endif
