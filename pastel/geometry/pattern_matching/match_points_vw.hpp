#ifndef PASTELGEOMETRY_MATCH_POINTS_VW_HPP
#define PASTELGEOMETRY_MATCH_POINTS_VW_HPP

#include "pastel/geometry/pattern_matching/match_points_vw.h"
#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/search_nearest.h"
#include "pastel/geometry/nearestset/kdtree_nearestset.h"
#include "pastel/geometry/bounding/bounding_sphere.h"

#include "pastel/math/affine/affine_transformation.h"
#include "pastel/math/conformalaffine2d/conformalaffine2d_tools.h"
#include "pastel/math/conformalaffine2d/conformalaffine2d_least_squares.h"

#include "pastel/sys/array.h"
#include "pastel/sys/output.h"
#include "pastel/sys/sequence/random_subset.h"
#include "pastel/sys/indicator/predicate_indicator.h"
#include "pastel/sys/vector/vector_locator.h"

namespace Pastel
{

	template <typename Point_ConstRange, typename Locator>
	typename Locator::Real 
		relativeToAbsoluteMatchingDistance(
		const Point_ConstRange& pointSet,
		const Locator& locator,
		const typename Locator::Real& relativeMatchingDistance)
	{
		using Real = typename Locator::Real;
		static constexpr integer N = Locator::N;

		Sphere<Real, N> sceneSphere = boundingSphere(
			pointSet, locator);

		return relativeMatchingDistance * 
			sceneSphere.radius() / (2 * std::sqrt((Real)pointSet.size()));
	}

	namespace MatchPointsVw_
	{

		template <
			typename Scene_Settings, template <typename> class Scene_Customization,
			typename Model_Settings, template <typename> class Model_Customization>
		class PatternMatcher
		{
		private:
			using Scene_Locator = typename Scene_Settings::Locator;
			using Model_Locator = typename Model_Settings::Locator;
			
			using Real = typename Scene_Locator::Real;
			static constexpr integer N = Scene_Locator::N;

			PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);

			using SceneTree = PointKdTree<Scene_Settings, Scene_Customization>;
			using SceneIterator = typename SceneTree::Point_ConstIterator;
			using ScenePoint = typename SceneTree::Point;

			using ModelTree = PointKdTree<Model_Settings, Model_Customization>;
			using ModelIterator = typename ModelTree::Point_ConstIterator;
			using ModelPoint = typename ModelTree::Point;

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
				, scenePoints_(sceneTree.points())
				, modelPoints_(modelTree.points())
				, k_(0)
				, k2_(0)
				, k3_(0)
				, matchingDistance2_(0)
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

				Real matchingFactorUpperBound =
					std::sqrt(minMatchRatio_ /
					(minMatchRatio_ + (std::sqrt((Real)5) - 1)));

				if (relativeMatchingDistance > matchingFactorUpperBound)
				{
					std::cout << "pointPatternMatchVw(): warning: the matching distance is greater than the upper bound"
						<< " to guarantee optimal asymptotic performance." << std::endl;
				}

				Real kSuggestion =
					std::log((Real)modelPoints_) /

					(2 * square(minMatchRatio - square(relativeMatchingDistance) / 4));
				Real k2Suggestion =
					std::log((Real)0.05) / std::log(1 - minMatchRatio);
				Real k3Suggestion =
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
				std::cout << "kSuggestion = " << kSuggestion << std::endl;
				std::cout << "k2Suggestion = " << k2Suggestion << std::endl;
				std::cout << "k3Suggestion = " << k3Suggestion << std::endl;
				std::cout << "k = " << k_ << std::endl;
				std::cout << "k2 = " << k2_ << std::endl;
				std::cout << "k3 = " << k3_ << std::endl;
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

			bool operator()(ConformalAffine2D<Real>& similarityResult) const
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
				integer modelPointsToTest = 
					std::ceil(clamp(
					std::log(1 - confidence_) / 
					std::log(1 - minMatchRatio_), 
					1, modelPoints_));
				randomSubset(
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
					integer j = i + 1 + randomInteger(scenePoints_ - (i + 1));
					std::swap(sceneIndexList[i], sceneIndexList[j]);
				}

				Array<SceneIterator> sceneNearest(Vector2i(k_, scenePoints_));
				std::vector<SceneIterator> sceneSet(k_ + 1);
				std::vector<ModelIterator> modelSet(k_ + 1);

				for (integer i = 0;i < modelPointsToTest;++i)
				{
					++modelPointTries_;

					ModelIterator modelIter = modelIndexList[i];

					for (integer j = 0;j < scenePoints_;++j)
					{
						++localTries_;

						// Find the k nearest neighbours
						// for both points in their respective point sets.

						SceneIterator sceneIter = sceneIndexList[j];
						sceneSet.front() = sceneIter;

						if (i == 0)
						{
							// Find the k nearest neighbors
							// for the scene point. Cache the
							// result.

							integer t = 0;

							auto report = [&](
								const Real& distance,
								SceneIterator point)
							{
								sceneNearest(t, j) = point;
								++t;
							};

							searchNearest(
								kdTreeNearestSet(sceneTree_), 
								scenePosition(sceneIter),
								PASTEL_TAG(report), report,
								PASTEL_TAG(accept), predicateIndicator(sceneIter, NotEqualTo()),
								PASTEL_TAG(kNearest), k_);
						}

						// Get the k-nearest neighbors from the cache.
						std::copy(
							sceneNearest.rowBegin(j),
							sceneNearest.rowEnd(j),
							sceneSet.begin() + 1);

						// Find the k-nearest neighbors for the model
						// point. These need not be reused, so we don't
						// need to cache them.

						modelSet.front() = modelIter;
						{
							integer t = 0;
							auto report = [&](
								const Real& distance,
								SceneIterator point)
							{
								modelSet[t + 1] = point;
								++t;
							};

							searchNearest(
								kdTreeNearestSet(modelTree_), 
								modelPosition(modelIter),
								PASTEL_TAG(report), report,
								PASTEL_TAG(accept), predicateIndicator(modelIter, NotEqualTo()),
								PASTEL_TAG(kNearest), k_);
						}

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
					const PointKdTree<Scene_Settings, Scene_Customization>& sceneTree)
					: sceneTree_(sceneTree)
				{
				}

				Vector<Real, N> operator()(const ScenePoint& scenePoint) const
				{
					return pointAsVector(location(scenePoint, sceneTree_.locator()));
				}

			private:
				const PointKdTree<Scene_Settings, Scene_Customization>& sceneTree_;
			};

			class SceneIteratorHash
			{
			public:
				std::size_t operator()(const SceneIterator& sceneIter) const
				{
					return computeHash(&sceneIter->point());
				}
			};

			Vector<Real, N> scenePosition(const SceneIterator& sceneIter) const
			{
				return pointAsVector(location(sceneIter->point(), sceneTree_.locator()));
			}

			Vector<Real, N> modelPosition(const ModelIterator& modelIter) const
			{
				return pointAsVector(location(modelIter->point(), modelTree_.locator()));
			}

			bool matchLocal(
				const std::vector<ModelIterator>& modelSet,
				const std::vector<SceneIterator>& sceneSet,
				ConformalAffine2D<Real>& resultSimilarity) const
			{
				ASSERT2(modelSet.size() == sceneSet.size(),
					modelSet.size(), sceneSet.size());

				// Note the modelSet and sceneSet contain
				// the pivot points in modelSet[0] and sceneSet[0],
				// respectively. Thus to access the k:th nearest
				// neighbour, one uses modelSet[k] and sceneSet[k].

				Vector<Real, N> modelPoint =
					modelPosition(modelSet[0]);
				Vector<Real, N> scenePoint =
					scenePosition(sceneSet[0]);

				for (integer i = k_ - k2_ + 1;i < k_ + 1;++i)
				{
					for (integer j = 0;j < k3_;++j)
					{
						ConformalAffine2D<Real> similarity =
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
							Vector<Real, N> transformedModelPoint =
								transformPoint(similarity, modelPosition(modelSet[m]));

							std::pair<Real, SceneIterator> closestScenePoint =
								searchNearest(
									kdTreeNearestSet(sceneTree_), 
									transformedModelPoint);

							// A transformed model point M' matches a scene point S
							// if the distance between M' and S is below
							// the matching threshold.
							// However, the paper suggest the use of

							// 2 * matchingDistance2.
							if (closestScenePoint.first <= 2 * matchingDistance2_)
							{
								modelMatch.push_back(modelPosition(modelSet[m]));
								sceneMatch.push_back(scenePosition(closestScenePoint.second));
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
				ConformalAffine2D<Real>& resultSimilarity) const
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

				ConformalAffine2D<Real> lsSimilarity;
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
							locationSet(
								modelGlobalMatch,
								Vector_Locator<Real, N>()
							),
							locationSet(
								sceneGlobalMatch,
								Vector_Locator<Real, N>()
							)
						);

					// Now see which of the mapped model points have
					// t-neighbours in the scene set.

					modelGlobalMatch.clear();
					sceneGlobalMatch.clear();

					// We need to make sure that no scene point
					// is paired to multiple model points.
					// Thus we keep track of which scene points
					// we have already paired.

					using UsedSceneSet = std::unordered_set<SceneIterator, SceneIteratorHash>;
					UsedSceneSet usedSet;

					ModelIterator modelIter = modelTree_.begin();
					ModelIterator modelEnd = modelTree_.end();
					while(modelIter != modelEnd)
					{
						Vector<Real, N> modelPoint =
							modelPosition(modelIter);

						Vector<Real, N> transformedModelPoint =
							transformPoint(lsSimilarity, modelPoint);

						// See if the model point maps near to some
						// scene point.

						std::pair<Real, SceneIterator> closestScenePoint =
							searchNearest(
								kdTreeNearestSet(sceneTree_), 
								transformedModelPoint);

						if (closestScenePoint.first <= matchingDistance2_ &&
							usedSet.find(closestScenePoint.second) == usedSet.end())
						{
							Vector<Real, N> scenePoint =
								scenePosition(closestScenePoint.second);

							// Add these points as a new matching pair.

							modelGlobalMatch.push_back(modelPoint);
							sceneGlobalMatch.push_back(scenePoint);

							// Mark this scene point as paired.

							usedSet.insert(closestScenePoint.second);
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
			Real minMatchRatio_;
			Real confidence_;
			integer scenePoints_;
			integer modelPoints_;
			integer k_;
			integer k2_;
			integer k3_;
			Real matchingDistance2_;

			mutable integer bestGlobalTry_;
			mutable integer bestLocalTry_;
			mutable integer localMatches_;
			mutable integer localTries_;
			mutable integer modelPointTries_;
		};

	}

	template <
		typename Scene_Settings, template <typename> class Scene_Customization,
		typename Model_Settings, template <typename> class Model_Customization,
		typename Real>
	bool pointPatternMatchVw(
		const PointKdTree<Scene_Settings, Scene_Customization>& sceneTree,
		const PointKdTree<Model_Settings, Model_Customization>& modelTree,
		const NoDeduction<Real>& minMatchRatio,
		const NoDeduction<Real>& relativeMatchingDistance,
		const NoDeduction<Real>& confidence,
		ConformalAffine2D<Real>& similarityResult)
	{
		ENSURE_OP(minMatchRatio, >=, 0); 
		ENSURE_OP(minMatchRatio, <=, 1);
		ENSURE_OP(relativeMatchingDistance, >=, 0);
		ENSURE_OP(confidence, >=, 0);
		ENSURE_OP(confidence, <=, 1);

		using SceneTree = PointKdTree<Scene_Settings, Scene_Customization>; 
		using ModelTree = PointKdTree<Model_Settings, Model_Customization>;

		MatchPointsVw_::PatternMatcher<
			Scene_Settings, Scene_Customization,
			Model_Settings, Model_Customization>
		patternMatcher(
			sceneTree, modelTree,
			minMatchRatio,
			relativeMatchingDistance,
			confidence);

		bool succeeded = patternMatcher(similarityResult);

		/*
		std::cout << "Local tries = " << patternMatcher.localTries() << std::endl;
		std::cout << "Best local try = " << patternMatcher.bestLocalTry() << std::endl;
		std::cout << "Global tries = " << patternMatcher.localMatches() << std::endl;
		std::cout << "Best global try = " << patternMatcher.bestGlobalTry() << std::endl;
		std::cout << "Model points tried = " << patternMatcher.modelPointTries() << std::endl;
		*/

		return succeeded;
	}

	template <
		typename Real, typename SceneRange, typename ModelRange,
		typename Scene_Locator, typename Model_Locator>
	bool pointPatternMatchVw(
		const SceneRange& scene,
		const ModelRange& model,
		const NoDeduction<Real>& minMatchRatio,
		const NoDeduction<Real>& relativeMatchingDistance,
		const NoDeduction<Real>& confidence,
		ConformalAffine2D<Real>& similarityResult,
		const Scene_Locator& sceneLocator,
		const Model_Locator& modelLocator)
	{
		ENSURE_OP(modelLocator.n(), ==, 2);
		ENSURE_OP(sceneLocator.n(), ==, 2);

		using SceneTree = PointKdTree<PointKdTree_Settings<Scene_Locator>>;
		using ModelTree = PointKdTree<PointKdTree_Settings<Model_Locator>>;

		SceneTree sceneTree(sceneLocator);
		sceneTree.insertSet(scene);

		ModelTree modelTree(modelLocator);
		modelTree.insertSet(model);

		sceneTree.refine(SlidingMidpoint_SplitRule());
		modelTree.refine(SlidingMidpoint_SplitRule());

		return Pastel::pointPatternMatchVw(
			sceneTree, modelTree, 
			minMatchRatio, relativeMatchingDistance,
			confidence, similarityResult);
	}

	template <typename Real, typename SceneRange, typename ModelRange>
	bool pointPatternMatchVw(
		const SceneRange& scene,
		const ModelRange& model,
		const NoDeduction<Real>& minMatchRatio,
		const NoDeduction<Real>& relativeMatchingDistance,
		const NoDeduction<Real>& confidence,
		ConformalAffine2D<Real>& similarityResult)
	{
		return Pastel::pointPatternMatchVw(
			scene, model,
			minMatchRatio, 
			relativeMatchingDistance,
			confidence,
			similarityResult,
			Vector_Locator<Real, 2>(),
			Vector_Locator<Real, 2>());
	}

}

#endif
