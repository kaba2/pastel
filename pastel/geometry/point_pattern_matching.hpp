#ifndef PASTEL_POINT_PATTERN_MATCHING_HPP
#define PASTEL_POINT_PATTERN_MATCHING_HPP

#include "pastel/geometry/point_pattern_matching.h"
#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"
#include "pastel/geometry/bounding_sphere.h"

#include "pastel/math/affinetransformation_tools.h"
#include "pastel/math/conformalaffine2d_tools.h"

#include "pastel/sys/unorderedset.h"
#include "pastel/sys/array.h"
#include "pastel/sys/smallset.h"
#include "pastel/sys/nulliterator.h"
#include "pastel/sys/stdext_subset.h"

namespace Pastel
{

	template <typename Point_ConstIterator, typename PointPolicy>
	typename PointPolicy::Coordinate 
		relativeToAbsoluteMatchingDistance(
		const ForwardRange<Point_ConstIterator>& pointSet,
		const PointPolicy& pointPolicy,
		const typename PointPolicy::Coordinate& relativeMatchingDistance)
	{
		typedef typename PointPolicy::Coordinate Real;
		enum
		{
			N = PointPolicy::N
		};

		const Sphere<Real, N> sceneSphere = boundingSphere(
			pointSet, pointPolicy);

		return relativeMatchingDistance * 
			sceneSphere.radius() / (2 * std::sqrt((Real)pointSet.size()));
	}

	namespace Detail_PointPatternMatch
	{

		template <typename Real, int N, typename ScenePolicy, typename ModelPolicy>
		class PatternMatcher
		{
		private:
			PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);

			typedef PointKdTree<Real, N, ScenePolicy> SceneTree;
			typedef typename SceneTree::ConstObjectIterator SceneIterator;
			typedef typename SceneTree::Object SceneObject;

			typedef PointKdTree<Real, N, ModelPolicy> ModelTree;
			typedef typename ModelTree::ConstObjectIterator ModelIterator;
			typedef typename ModelTree::Object ModelObject;

		public:
			PatternMatcher(
				const SceneTree& sceneTree,
				const ModelTree& modelTree,
				const Real& minMatchRatio,
				const Real& relativeMatchingDistance,
				const Real& confidence)
				: sceneTree_(sceneTree)
				, modelTree_(modelTree)
				, minMatchRatio_(minMatchRatio)
				, confidence_(confidence)
				, scenePoints_(sceneTree.objects())
				, modelPoints_(modelTree.objects())
				, k_(0)
				, k2_(0)
				, k3_(0)
				, matchingDistance_(0)
				, bestGlobalTry_(0)
				, bestLocalTry_(0)
				, localMatches_(0)
				, localTries_(0)
				, modelPointTries_(0)
			{
				// 'localMatches' refers to the number of points
				// where the neighborhood matches locally.
				// 'localTries' refers to the number of points
				// whose neighborhood have been tested.

				// The algorithm we use here is from the paper:
				// "A fast expected time algorithm for the 2-D point pattern
				// matching problem", P.B. Van Wamelen et al.,
				// Pattern Recognition 37 (2004), 1699-1711.

				// An upper bound for matching factor.

				const Real matchingFactorUpperBound =
					std::sqrt(minMatchRatio_ /
					(minMatchRatio_ + (std::sqrt((Real)5) - 1)));

				if (relativeMatchingDistance > matchingFactorUpperBound)
				{
					log() << "pointPatternMatch(): warning: the matching distance is greater than the upper bound"
						<< " to guarantee optimal asymptotic performance." << logNewLine;
				}

				const Real kSuggestion =
					std::log((Real)modelPoints_) /
					(2 * square(minMatchRatio - square(relativeMatchingDistance) / 4));
				const Real k2Suggestion =
					std::log((Real)0.05) / std::log(1 - minMatchRatio);
				const Real k3Suggestion =
					2 * relativeMatchingDistance * std::sqrt(kSuggestion / constantPi<Real>());

				// We need to find at least one neighbor.
				k_ = clamp((integer)std::ceil(kSuggestion), 
					1, std::min(modelPoints_, scenePoints_) - 1);
				// We want k3 to be odd, so that the window is symmetric.
				// It must be at least 1, but of course not greater than 'k_'.
				// roundUpToEven(k_) - 1 = roundDownToOdd(k_).
				k3_ = clamp(roundUpToOdd((integer)std::ceil(k3Suggestion)), 
					1, roundUpToEven(k_) - 1);
				// The k2 must be chosen such that the window keeps inside
				// the proper range. That is:
				// (k - k2) - k3 / 2 >= 0
				// <=>
				// k2 >= k - k3 / 2
				// Of course k2 must be at least 1.
				k2_ = clamp((integer)std::ceil(k2Suggestion), 
					1, k_ - k3_ / 2);

				/*
				log() << "kSuggestion = " << kSuggestion << logNewLine;
				log() << "k2Suggestion = " << k2Suggestion << logNewLine;
				log() << "k3Suggestion = " << k3Suggestion << logNewLine;
				log() << "k = " << k_ << logNewLine;
				log() << "k2 = " << k2_ << logNewLine;
				log() << "k3 = " << k3_ << logNewLine;
				*/
			}

			integer bestLocalTry() const
			{
				return bestLocalTry_;
			}

			integer bestGlobalTry() const
			{
				return bestGlobalTry_;
			}

			integer localMatches() const
			{
				return localMatches_;
			}

			integer localTries() const
			{
				return localTries_;
			}

			integer modelPointTries() const
			{
				return modelPointTries_;
			}

			bool operator()(ConformalAffine2D<Real, N>& similarityResult) const
			{
				bestLocalTry_ = 0;
				bestGlobalTry_ = 0;
				localMatches_ = 0;
				localTries_ = 0;
				modelPointTries_ = 0;

				// Find a permuted order for model points.

				std::vector<ModelIterator> modelIndexList;
				modelIndexList.reserve(modelPoints_);
				std::copy(
					countingIterator(modelTree_.begin()),
					countingIterator(modelTree_.end()),
					std::back_inserter(modelIndexList));
				
				// We only need those points randomized
				// which we are going to use as a pivot point.
				const integer modelPointsToTest = 
					std::ceil(clamp(
					std::log(1 - confidence_) / 
					std::log(1 - minMatchRatio_), 
					1, modelPoints_));
				StdExt::subset(
					modelIndexList.begin(),
					modelIndexList.end(),
					modelPointsToTest);

				// Randomize all of the scene pointset.
				std::vector<SceneIterator> sceneIndexList;
				sceneIndexList.reserve(scenePoints_);
				std::copy(
					countingIterator(sceneTree_.begin()),
					countingIterator(sceneTree_.end()),
					std::back_inserter(sceneIndexList));
				for (integer i = 0;i < scenePoints_ - 1;++i)
				{
					const integer j = i + 1 + (randomInteger() % (scenePoints_ - (i + 1)));
					std::swap(sceneIndexList[i], sceneIndexList[j]);
				}

				Array<SceneIterator> sceneNearest(k_, scenePoints_);
				std::vector<SceneIterator> sceneSet(k_ + 1);
				std::vector<ModelIterator> modelSet(k_ + 1);

				for (integer i = 0;i < modelPointsToTest;++i)
				{
					++modelPointTries_;

					const ModelIterator modelIter = modelIndexList[i];

					for (integer j = 0;j < scenePoints_;++j)
					{
						++localTries_;

						// Find the k nearest neighbours
						// for both points in their respective point sets.

						const SceneIterator sceneIter = sceneIndexList[j];
						sceneSet.front() = sceneIter;

						if (i == 0)
						{
							// Find the k nearest neighbors
							// for the scene point. Cache the
							// result.

							searchNearest(
								sceneTree_,
								sceneIter,
								k_,
								sceneNearest.rowBegin(j),
								NullIterator(),
								infinity<Real>(), 0,
								Dont_AcceptPoint<SceneIterator>(sceneIter));
						}

						// Get the k-nearest neighbors from the cache.
						std::copy(
							sceneNearest.rowBegin(j),
							sceneNearest.rowEnd(j),
							sceneSet.begin() + 1);

						// Find the k-nearest neighbors for the model
						// point. These need not reused, so we don't
						// need to cache them.

						modelSet.front() = modelIter;
						searchNearest(
							modelTree_,
							modelIter,
							k_,
							modelSet.begin() + 1,
							NullIterator(),
							infinity<Real>(), 0,
							Dont_AcceptPoint<ModelIterator>(modelIter));

						// Try to match the nearest neighbours.
						// If they match, then try to improve the
						// transform to a global result.

						if (matchLocal(modelSet, sceneSet,
							similarityResult))
						{
							return true;
						}
					}
				}

				return false;
			}

		private:
			class ScenePositionFunctor
			{
			public:
				explicit ScenePositionFunctor(
					const PointKdTree<Real, N, ScenePolicy>& sceneTree)
					: sceneTree_(sceneTree)
				{
				}

				Vector<Real, N> operator()(const SceneObject& sceneObject) const
				{
					return Vector<Real, N>(
						ofDimension(sceneTree_.dimension()),
						withAliasing((Real*)sceneTree_.pointPolicy().point(sceneObject)));
				}

			private:
				const PointKdTree<Real, N, ScenePolicy>& sceneTree_;
			};

			class SceneIteratorHash
			{
			public:
				std::size_t operator()(const SceneIterator& sceneIter) const
				{
					const hash<SceneObject*> hash;
					return hash(&sceneIter->object());
				}
			};

			Vector<Real, N> scenePosition(const SceneIterator& sceneIter) const
			{
				return sceneTree_.pointPolicy()(sceneIter->object());
			}

			Vector<Real, N> modelPosition(const ModelIterator& modelIter) const
			{
				return modelTree_.pointPolicy()(modelIter->object());
			}

			bool matchLocal(
				const std::vector<ModelIterator>& modelSet,
				const std::vector<SceneIterator>& sceneSet,
				ConformalAffine2D<Real, N>& resultSimilarity) const
			{
				ASSERT2(modelSet.size() == sceneSet.size(),
					modelSet.size(), sceneSet.size());

				// Note the modelSet and sceneSet contain
				// the pivot points in modelSet[0] and sceneSet[0],
				// respectively. Thus to access the k:th nearest
				// neighbour, one uses modelSet[k] and sceneSet[k].

				const Vector<Real, N> modelPoint =
					modelPosition(modelSet[0]);
				const Vector<Real, N> scenePoint =
					scenePosition(sceneSet[0]);

				for (integer i = k_ - k2_ + 1;i < k_ + 1;++i)
				{
					for (integer j = 0;j < k3_;++j)
					{
						const ConformalAffine2D<Real, N> similarity =
							conformalAffine(
							modelPoint, modelPosition(modelSet[i - k3_ / 2]),
							scenePoint, scenePosition(sceneSet[i - j]));

						// Count the number of points this similarity transform
						// matches between the local point sets.

						std::vector<Vector<Real, N> > modelMatch;
						modelMatch.push_back(modelPoint);
						std::vector<Vector<Real, N> > sceneMatch;
						sceneMatch.push_back(scenePoint);

						for (integer m = 1;m < k_ + 1;++m)
						{
							const Vector<Real, N> transformedModelPoint =
								transformPoint(modelPosition(modelSet[m]), similarity);

							const KeyValue<Real, SceneIterator> closestScenePoint =
								searchNearestOne(sceneTree_, transformedModelPoint);

							// A transformed model point M' matches a scene point S
							// if the distance between M' and S is below
							// the matching threshold.
							// However, the paper suggest the use of
							// 2 * matchingDistance.
							if (closestScenePoint.key() <= 2 * matchingDistance_)
							{
								modelMatch.push_back(modelPosition(modelSet[m]));
								sceneMatch.push_back(scenePosition(closestScenePoint.value()));
							}
						}

						// See if enough of the points matched.

						if (modelMatch.size() > bestLocalTry_)
						{
							bestLocalTry_ = modelMatch.size();
						}

						//if (modelMatch.size() - 2 >= minMatchRatio_ * (k_ - 1))
						if (modelMatch.size() >= minMatchRatio_ * k_)
						{
							// A local match found.
							// Try to improve to a global match.

							if (improveGlobal(modelMatch, sceneMatch,
								resultSimilarity))
							{
								return true;
							}
						}
					}
				}

				return false;
			}

			bool improveGlobal(
				const std::vector<Vector<Real, N> >& modelMatch,
				const std::vector<Vector<Real, N> >& sceneMatch,
				ConformalAffine2D<Real, N>& resultSimilarity) const
			{
				ASSERT_OP(sceneMatch.size(), ==, modelMatch.size());

				if (modelMatch.size() == 0)
				{
					return false;
				}

				++localMatches_;

				std::vector<Vector<Real, N> > modelGlobalMatch(modelMatch);
				std::vector<Vector<Real, N> > sceneGlobalMatch(sceneMatch);

				integer matches = 0;

				ConformalAffine2D<Real, N> lsSimilarity;
				// We want to go through the improving process
				// at least once even if we already had enough
				// matching points. This is because at each
				// iteration we compute the least squares similarity
				// transformation which might improve the solution.
				do
				{
					// Find the least squares similarity transform
					// of the current matching sets.

					lsSimilarity =
						lsConformalAffine(
						forwardRange(modelGlobalMatch.begin(), modelGlobalMatch.end()),
						forwardRange(sceneGlobalMatch.begin(), sceneGlobalMatch.end()),
						Vector_PointPolicy<Real, N>(),
						Vector_PointPolicy<Real, N>());

					// Now see which of the mapped model points have
					// t-neighbours in the scene set.

					modelGlobalMatch.clear();
					sceneGlobalMatch.clear();

					// We need to make sure that no scene point
					// is paired to multiple model points.
					// Thus we keep track of which scene points
					// we have already paired.

					typedef UnorderedSet<SceneIterator, SceneIteratorHash> UsedSceneSet;
					UsedSceneSet usedSet;

					ModelIterator modelIter = modelTree_.begin();
					const ModelIterator modelEnd = modelTree_.end();
					while(modelIter != modelEnd)
					{
						const Vector<Real, N> modelPoint =
							modelPosition(modelIter);

						const Vector<Real, N> transformedModelPoint =
							transformPoint(modelPoint, lsSimilarity);

						// See if the model point maps near to some
						// scene point.

						const KeyValue<Real, SceneIterator> closestScenePoint =
							searchNearestOne(sceneTree_, transformedModelPoint);

						if (closestScenePoint.key() <= matchingDistance_ &&
							usedSet.find(closestScenePoint.value()) == usedSet.end())
						{
							const Vector<Real, N> scenePoint =
								scenePosition(closestScenePoint.value());

							// Add these points as a new matching pair.

							modelGlobalMatch.push_back(modelPoint);
							sceneGlobalMatch.push_back(scenePoint);

							// Mark this scene point as paired.

							usedSet.insert(closestScenePoint.value());
						}

						++modelIter;
					}

					// If the solution did not extend anymore,
					// give up.
					if (sceneGlobalMatch.size() <= matches)
					{
						return false;
					}

					matches = sceneGlobalMatch.size();
					if (matches > bestGlobalTry_)
					{
						bestGlobalTry_ = matches;
					}
				}
				while(matches < minMatchRatio_ * modelPoints_);

				resultSimilarity = lsSimilarity;

				return true;
			}

		private:
			const SceneTree& sceneTree_;
			const ModelTree& modelTree_;
			const Real minMatchRatio_;
			const Real confidence_;
			const integer scenePoints_;
			const integer modelPoints_;
			integer k_;
			integer k2_;
			integer k3_;
			Real matchingDistance_;

			mutable integer bestGlobalTry_;
			mutable integer bestLocalTry_;
			mutable integer localMatches_;
			mutable integer localTries_;
			mutable integer modelPointTries_;
		};

	}

	template <typename Real, int N, typename ScenePolicy, typename ModelPolicy>
	bool pointPatternMatch(
		const PointKdTree<Real, N, ScenePolicy>& sceneTree,
		const PointKdTree<Real, N, ModelPolicy>& modelTree,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& relativeMatchingDistance,
		const PASTEL_NO_DEDUCTION(Real)& confidence,
		ConformalAffine2D<Real, N>& similarityResult)
	{
		ENSURE_OP(minMatchRatio, >=, 0); 
		ENSURE_OP(minMatchRatio, <=, 1);
		ENSURE_OP(relativeMatchingDistance, >=, 0);
		ENSURE_OP(relativeMatchingDistance, <=, 1);
		ENSURE_OP(confidence, >=, 0);
		ENSURE_OP(confidence, <=, 1);

		Detail_PointPatternMatch::PatternMatcher<Real, N, ScenePolicy, ModelPolicy>
			patternMatcher(
			sceneTree, modelTree,
			minMatchRatio,
			relativeMatchingDistance,
			confidence);

		const bool succeeded = patternMatcher(similarityResult);

		/*
		log() << "Local tries = " << patternMatcher.localTries() << logNewLine;
		log() << "Best local try = " << patternMatcher.bestLocalTry() << logNewLine;
		log() << "Global tries = " << patternMatcher.localMatches() << logNewLine;
		log() << "Best global try = " << patternMatcher.bestGlobalTry() << logNewLine;
		log() << "Model points tried = " << patternMatcher.modelPointTries() << logNewLine;
		*/

		return succeeded;
	}

	template <typename Real, int N, typename SceneIterator, typename ModelIterator,
		typename Model_PointPolicy, typename Scene_PointPolicy>
	bool pointPatternMatch(
		const ForwardRange<SceneIterator>& scene,
		const ForwardRange<ModelIterator>& model,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& relativeMatchingDistance,
		const PASTEL_NO_DEDUCTION(Real)& confidence,
		ConformalAffine2D<Real, N>& similarityResult,
		const Model_PointPolicy& modelPointPolicy,
		const Scene_PointPolicy& scenePointPolicy)
	{
		ENSURE_OP(modelPointPolicy.dimension(), ==, 2);
		ENSURE_OP(scenePointPolicy.dimension(), ==, 2);

		typedef PointKdTree<Real, N, Model_PointPolicy> SceneTree;
		typedef SceneTree::ConstObjectIterator SceneIterator;

		typedef PointKdTree<Real, N, Scene_PointPolicy> ModelTree;
		typedef ModelTree::ConstObjectIterator ModelIterator;

		SceneTree sceneTree(false, scenePointPolicy);
		sceneTree.insert(scene);

		ModelTree modelTree(false, modelPointPolicy);
		modelTree.insert(model);

		sceneTree.refine(SlidingMidpoint_SplitRule_PointKdTree());
		modelTree.refine(SlidingMidpoint_SplitRule_PointKdTree());

		return Pastel::pointPatternMatch(
			sceneTree, modelTree, 
			minMatchRatio, relativeMatchingDistance,
			confidence, similarityResult);
	}

	template <typename Real, int N, typename SceneIterator, typename ModelIterator>
	bool pointPatternMatch(
		const ForwardRange<SceneIterator>& scene,
		const ForwardRange<ModelIterator>& model,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& relativeMatchingDistance,
		const PASTEL_NO_DEDUCTION(Real)& confidence,
		ConformalAffine2D<Real, N>& similarityResult)
	{
		return Pastel::pointPatternMatch(
			scene, model,
			minMatchRatio, relativeMatchingDistance,
			confidence,
			similarityResult,
			Vector_PointPolicy<Real, N>(),
			Vector_PointPolicy<Real, N>());
	}

}

#endif
