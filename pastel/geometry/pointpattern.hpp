#ifndef PASTELGEOMETRY_POINTPATTERN_HPP
#define PASTELGEOMETRY_POINTPATTERN_HPP

#include "pastel/geometry/pointpattern.h"
#include "pastel/geometry/kdtree.h"
#include "pastel/geometry/kdtree_tools.h"
#include "pastel/geometry/bounding_sphere.h"

#include "pastel/math/affinetransformation_tools.h"

#include "pastel/sys/unorderedset.h"
#include "pastel/sys/array.h"
#include "pastel/sys/smallset.h"

namespace Pastel
{

	namespace Detail_PointPatternMatch
	{

		template <typename Real, typename ScenePolicy, typename ModelPolicy>
		class PatternMatcher
		{
		private:
			typedef KdTree<2, Real, ScenePolicy> SceneTree;
			typedef typename SceneTree::ConstObjectIterator SceneIterator;
			typedef typename SceneTree::Object SceneObject;

			typedef KdTree<2, Real, ModelPolicy> ModelTree;
			typedef typename ModelTree::ConstObjectIterator ModelIterator;
			typedef typename ModelTree::Object ModelObject;

		public:
			PatternMatcher(
				const KdTree<2, Real, ScenePolicy>& sceneTree,
				const KdTree<2, Real, ModelPolicy>& modelTree,
				const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
				const PASTEL_NO_DEDUCTION(Real)& matchingDistance,
				const PatternMatch::Enum& matchingDistanceType)
				: sceneTree_(sceneTree)
				, modelTree_(modelTree)
				, minMatchRatio_(minMatchRatio)
				, scenePoints_(sceneTree.objects())
				, modelPoints_(modelTree.objects())
				, kPoints_(0)
				, k2Points_(0)
				, k3Points_(0)
				, matchingThreshold_(0)
				, matchingFactor_(0)
				, bestGlobalTry_(0)
				, bestLocalTry_(0)
				, globalTries_(0)
				, localTries_(0)
				, modelPointTries_(0)
			{
				// 'globalTries' refers to the number of points
				// where the neighborhood matches locally.
				// 'localTries' refers to the number of points
				// whose neighborhood have been tested.

				// The algorithm we use here is from the paper:
				// "A fast expected time algorithm for the 2-D point pattern
				// matching problem", P.B. Van Wamelen et al.,
				// Pattern Recognition 37 (2004), 1699-1711.

				const ScenePositionFunctor scenePositionFunctor(sceneTree);
				const Sphere<2, Real> sceneSphere = boundingSphere<2, Real>(
					sceneTree.begin(), sceneTree.end(), scenePositionFunctor);


				if (matchingDistanceType == PatternMatch::AbsoluteDistance)
				{
					matchingThreshold_ = matchingDistance;
					matchingFactor_ =
						2 * matchingThreshold_ * std::sqrt((Real)scenePoints_) /
						sceneSphere.radius();
				}
				else
				{
					//matchingFactor_ = std::min(matchingDistance, matchingFactorUpperBound);
					matchingFactor_ = matchingDistance;
					matchingThreshold_ =
						matchingFactor_ * sceneSphere.radius() /
						(2 * std::sqrt((Real)scenePoints_));
				}

				// An upper bound for matching factor.

				const Real matchingFactorUpperBound =
					std::sqrt(minMatchRatio_ /
					(minMatchRatio_ + (std::sqrt((Real)5) - 1)));

				if (matchingFactor_ > matchingFactorUpperBound)
				{
					log() << "pointPatternMatch(): warning: the matching distance is greater than the upper bound"
						<< " to guarantee optimal asymptotic performance." << logNewLine;
				}

				const Real kConstant =
					std::log((Real)modelPoints_) /
					(2 * square(minMatchRatio - square(matchingFactor_) / 4));
				const Real k2Constant =
					std::log(0.05) / std::log(1 - minMatchRatio);
				const Real k3Constant =
					2 * matchingFactor_ * std::sqrt(kConstant / constantPi<Real>());

				kPoints_ = std::min((integer)std::ceil(kConstant), modelPoints_ - 1);
				k2Points_ = std::max(std::min((integer)std::ceil(k2Constant), kPoints_), 1);
				k3Points_ = std::max(std::min((integer)std::ceil(k3Constant), kPoints_ - k2Points_ + 1), 1);

				log() << "k = " << kPoints_ << logNewLine;
				log() << "k2 = " << k2Points_ << logNewLine;
				log() << "k3 = " << k3Points_ << logNewLine;
			}

			integer bestLocalTry() const
			{
				return bestLocalTry_;
			}

			integer bestGlobalTry() const
			{
				return bestGlobalTry_;
			}

			integer globalTries() const
			{
				return globalTries_;
			}

			integer localTries() const
			{
				return localTries_;
			}

			integer modelPointTries() const
			{
				return modelPointTries_;
			}

			bool operator()(Tuple<4, Real>& similarityResult) const
			{
				bestLocalTry_ = 0;
				bestGlobalTry_ = 0;
				globalTries_ = 0;
				localTries_ = 0;
				modelPointTries_ = 0;

				// We want to try the model points in a random
				// permuted order.

				std::vector<ModelIterator> modelIndexList;

				// Since the objects are points, all
				// objects are listed only once in the kdtree's
				// object list.
				{
					modelIndexList.reserve(modelPoints_);

					ModelIterator modelIter = modelTree_.begin();
					const ModelIterator modelEnd = modelTree_.end();
					while(modelIter != modelEnd)
					{
						modelIndexList.push_back(modelIter);
						++modelIter;
					}
				}

				// Permute the index list.
				for (integer i = 0;i < modelPoints_;++i)
				{
					std::swap(modelIndexList[i],
						modelIndexList[randomInteger() % modelPoints_]);
				}

				for (integer i = 0;i < modelPoints_;++i)
				{
					++modelPointTries_;

					const ModelObject& modelObject =
						*modelIndexList[i];
					const Point<2, Real> modelPoint =
						modelTree_.objectPolicy().bound(modelObject).min();

					SceneIterator sceneIter = sceneTree_.begin();
					const SceneIterator sceneEnd = sceneTree_.end();

					while(sceneIter != sceneEnd)
					{
						++localTries_;

						const SceneObject& sceneObject =
							*sceneIter;
						const Point<2, Real> scenePoint =
							sceneTree_.objectPolicy().bound(sceneObject).min();

						// Find the k nearest neighbours
						// for both points in their respective point sets.
						// Because k is smaller than the number of
						// points in either point set and the search radius
						// is infinite, both searches should return exactly
						// k result points.
						// Note: we search for k + 1 points because
						// the pivot point will be part of the result.

						SmallSet<KeyValue<Real, SceneIterator> > sceneSet;
						searchNearest(
							sceneTree_,
							scenePoint,
							infinity<Real>(),
							0,
							EuclideanNormBijection<2, Real>(),
							kPoints_ + 1,
							sceneSet);

						SmallSet<KeyValue<Real, ModelIterator> > modelSet;
						searchNearest(
							modelTree_,
							modelPoint,
							infinity<Real>(),
							0,
							EuclideanNormBijection<2, Real>(),
							kPoints_ + 1,
							modelSet);

						// Try to match the nearest neighbours.
						// If they match, then try to improve the
						// transform to a global result.

						if (matchLocal(
							modelSet, sceneSet,
							similarityResult))
						{
							return true;
						}

						++sceneIter;
					}
				}

				return false;
			}

		private:
			class ScenePositionFunctor
			{
			public:
				explicit ScenePositionFunctor(
					const KdTree<2, Real, ScenePolicy>& sceneTree)
					: sceneTree_(sceneTree)
				{
				}

				Point<2, Real> operator()(const SceneObject& sceneObject) const
				{
					return sceneTree_.objectPolicy().bound(sceneObject).min();
				}

			private:
				const KdTree<2, Real, ScenePolicy>& sceneTree_;
			};

			class SceneIteratorHash
			{
			public:
				std::size_t operator()(const SceneIterator& sceneIter) const
				{
					const hash<SceneObject*> hash;
					return hash(&*sceneIter);
				}
			};

			Point<2, Real> scenePosition(const SceneIterator& sceneIter) const
			{
				return sceneTree_.objectPolicy().bound(*sceneIter).min();
			}

			Point<2, Real> modelPosition(const ModelIterator& modelIter) const
			{
				return modelTree_.objectPolicy().bound(*modelIter).min();
			}

			bool matchLocal(
				const SmallSet<KeyValue<Real, ModelIterator> >& modelSet,
				const SmallSet<KeyValue<Real, SceneIterator> >& sceneSet,
				Tuple<4, Real>& resultSimilarity) const
			{
				ASSERT2(modelSet.size() == sceneSet.size(),
					modelSet.size(), sceneSet.size());

				// Note the modelSet and sceneSet contain
				// the pivot points in modelSet[0] and sceneSet[0],
				// respectively. Thus to access the k:th nearest
				// neighbour, one uses modelSet[k] and sceneSet[k].

				const Point<2, Real> modelPoint =
					modelPosition(modelSet[0].value());
				const Point<2, Real> scenePoint =
					scenePosition(sceneSet[0].value());

				for (integer i = kPoints_ - k2Points_ + 1;i < kPoints_ + 1;++i)
				{
					for (integer j = 0;j < k3Points_;++j)
					{
						const Tuple<4, Real> similarityParameters =
							similarityTransformation(
							modelPoint, modelPosition(modelSet[i - k3Points_ / 2].value()),
							scenePoint, scenePosition(sceneSet[i - j].value()));

						const AffineTransformation<2, Real> similarity =
							similarityTransformation(similarityParameters);

						// Count the number of points this similarity transform
						// matches between the local point sets.

						std::vector<Point<2, Real> > modelMatch;
						modelMatch.push_back(modelPoint);
						std::vector<Point<2, Real> > sceneMatch;
						sceneMatch.push_back(scenePoint);

						for (integer m = 1;m < kPoints_ + 1;++m)
						{
							const Point<2, Real> transformedModelPoint =
								modelPosition(modelSet[m].value()) * similarity;

							const KeyValue<Real, SceneIterator> closestScenePoint =
								searchNearest(sceneTree_, transformedModelPoint);

							// A transformed model point M' matches a scene point S
							// if the distance between M' and S is below
							// the matching threshold.
							// However, the paper suggest the use of
							// 2 * matchingThreshold.
							if (closestScenePoint.key() <= 2 * matchingThreshold_)
							{
								modelMatch.push_back(modelPosition(modelSet[m].value()));
								sceneMatch.push_back(scenePosition(closestScenePoint.value()));
							}
						}

						// See if enough of the points matched.

						if (modelMatch.size() > bestLocalTry_)
						{
							bestLocalTry_ = modelMatch.size();
						}

						//if (modelMatch.size() - 2 >= minMatchRatio_ * (kPoints_ - 1))
						if (modelMatch.size() >= minMatchRatio_ * kPoints_)
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
				const std::vector<Point<2, Real> >& modelMatch,
				const std::vector<Point<2, Real> >& sceneMatch,
				Tuple<4, Real>& resultSimilarity) const
			{
				ASSERT2(sceneMatch.size() == modelMatch.size(),
					sceneMatch.size(), modelMatch.size());

				++globalTries_;

				std::vector<Point<2, Real> > modelGlobalMatch(modelMatch);
				std::vector<Point<2, Real> > sceneGlobalMatch(sceneMatch);

				integer matches = 0;

				Tuple<4, Real> lsSimilarityParameters;
				// We want to go through the improving process
				// at least once even if we already had enough
				// matching points. This is because at each
				// iteration we compute the least squares similarity
				// transformation which might improve the solution.
				do
				{
					// Find the least squares similarity transform
					// of the current matching sets.

					lsSimilarityParameters =
						similarityTransformation(modelGlobalMatch, sceneGlobalMatch);

					const AffineTransformation<2, Real> lsSimilarity =
						similarityTransformation(lsSimilarityParameters);

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
						const Point<2, Real> modelPoint =
							modelPosition(modelIter);

						const Point<2, Real> transformedModelPoint =
							modelPoint * lsSimilarity;

						// See if the model point maps near to some
						// scene point.

						const KeyValue<Real, SceneIterator> closestScenePoint =
							searchNearest(sceneTree_, transformedModelPoint);

						if (closestScenePoint.key() <= matchingThreshold_ &&
							usedSet.find(closestScenePoint.value()) == usedSet.end())
						{
							const Point<2, Real> scenePoint =
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

				resultSimilarity = lsSimilarityParameters;

				return true;
			}

		private:
			const KdTree<2, Real, ScenePolicy>& sceneTree_;
			const KdTree<2, Real, ModelPolicy>& modelTree_;
			const Real minMatchRatio_;
			const integer scenePoints_;
			const integer modelPoints_;
			integer kPoints_;
			integer k2Points_;
			integer k3Points_;
			Real matchingThreshold_;
			Real matchingFactor_;

			mutable integer bestGlobalTry_;
			mutable integer bestLocalTry_;
			mutable integer globalTries_;
			mutable integer localTries_;
			mutable integer modelPointTries_;
		};

	}

	template <typename Real, typename ScenePolicy, typename ModelPolicy>
	bool pointPatternMatch(
		const KdTree<2, Real, ScenePolicy>& sceneTree,
		const KdTree<2, Real, ModelPolicy>& modelTree,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& matchingDistance,
		const PatternMatch::Enum& matchingDistanceType,
		Tuple<4, Real>& similarityResult)
	{
		ENSURE1(minMatchRatio >= 0 && minMatchRatio <= 1, minMatchRatio);
		if (matchingDistanceType == PatternMatch::AbsoluteDistance)
		{
			ENSURE1(matchingDistanceType == PatternMatch::AbsoluteDistance &&
				matchingDistance >= 0, matchingDistance);
		}
		else if (matchingDistanceType == PatternMatch::RelativeDistance)
		{
			ENSURE1(matchingDistanceType == PatternMatch::RelativeDistance &&
				matchingDistance >= 0 && matchingDistance <= 1, matchingDistance);
		}

		Detail_PointPatternMatch::PatternMatcher<Real, ScenePolicy, ModelPolicy>
			patternMatcher(
			sceneTree, modelTree,
			minMatchRatio,
			matchingDistance,
			matchingDistanceType);

		const bool succeeded = patternMatcher(similarityResult);

		log() << "Local tries = " << patternMatcher.localTries() << logNewLine;
		log() << "Best local try = " << patternMatcher.bestLocalTry() << logNewLine;
		log() << "Global tries = " << patternMatcher.globalTries() << logNewLine;
		log() << "Best global try = " << patternMatcher.bestGlobalTry() << logNewLine;
		log() << "Model points tried = " << patternMatcher.modelPointTries() << logNewLine;

		return succeeded;
	}

	template <typename Real, typename SceneIterator, typename ModelIterator>
	bool pointPatternMatch(
		const SceneIterator& sceneBegin,
		const SceneIterator& sceneEnd,
		const ModelIterator& modelBegin,
		const ModelIterator& modelEnd,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& matchingDistance,
		const PatternMatch::Enum& matchingDistanceType,
		Tuple<4, Real>& similarityResult)
	{
		typedef KdTree<2, Real> SceneTree;
		typedef SceneTree::ConstObjectIterator SceneIterator;

		typedef KdTree<2, Real> ModelTree;
		typedef ModelTree::ConstObjectIterator ModelIterator;

		SceneTree sceneTree;
		sceneTree.insert(sceneBegin, sceneEnd);

		ModelTree modelTree;
		modelTree.insert(modelBegin, modelEnd);

		sceneTree.refine(
			computeKdTreeMaxDepth(sceneTree.objects()), 4, SlidingMidpointRule());
		modelTree.refine(
			computeKdTreeMaxDepth(modelTree.objects()), 4, SlidingMidpointRule());

		return Pastel::pointPatternMatch(
			sceneTree, modelTree, minMatchRatio,  matchingDistance,
			matchingDistanceType, similarityResult);
	}

}

#endif
