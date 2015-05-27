#ifndef PASTELGEOMETRY_POINTKDTREE_SEARCH_NEAREST_ALGORITHM_HPP
#define PASTELGEOMETRY_POINTKDTREE_SEARCH_NEAREST_ALGORITHM_HPP

#include "pastel/geometry/pointkdtree/pointkdtree_search_nearest_algorithm.h"
#include "pastel/geometry/distance/distance_point_point.h"
#include "pastel/geometry/distance/distance_alignedbox_point.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/locator/vector_locator.h"

#include <queue>

namespace Pastel
{

	namespace NearestAlgorithm_
	{

		template <typename IntervalSequence>
		class ToIndexSequence
		{
		};

		template <typename Type, int N>
		class ToIndexSequence<Vector<Type, N>>
		{
		public:
			using type = Vector<integer, N>;
		};

		template <
			typename KdTree,
			typename Indicator, 
			typename NormBijection, 
			typename CandidateFunctor, 
			typename SearchAlgorithm_PointKdTree,
			typename IntervalSequence>
		class GenericAlgorithm
		{
		private:
			using Fwd = KdTree;
			PASTEL_FWD(Locator);
			using Real = typename Locator::Real;
			static constexpr integer N = Locator::N;
			PASTEL_FWD(Cursor);
			PASTEL_FWD(Point_ConstIterator);

			using IndexSequence = 
				typename ToIndexSequence<IntervalSequence>::type;

			struct State
			: boost::less_than_comparable<State>
			{
				State(
					const Cursor& cursor_,
					const Real& distance_,
					const IndexSequence& indexSequence_)
				: cursor(cursor_)
				, distance(distance_)
				, indexSequence(indexSequence_)
				{
				}

				bool operator<(const State& that) const
				{
					return distance < that.distance;
				}

				Cursor cursor;
				Real distance;
				IndexSequence indexSequence;
			};

			typedef typename SearchAlgorithm_PointKdTree::template Instance<State>
				SearchAlgorithm;

		public:
			GenericAlgorithm(
				const KdTree& kdTree_,
				const Vector<Real, N>& searchPoint_,
				const typename KdTree::Point_ConstIterator& searchIter_,
				const Real& maxDistance_,
				const Real& maxRelativeError_,
				const Indicator& acceptPoint_,
				integer bucketSize_,
				const NormBijection& normBijection_,
				const CandidateFunctor& candidateFunctor_,
				const IntervalSequence& timeIntervalSequence_)
				: kdTree(kdTree_)
				, searchPoint(searchPoint_)
				, searchIter(searchIter_)
				, maxDistance(maxDistance_)
				, maxRelativeError(maxRelativeError_)
				, acceptPoint(acceptPoint_)
				, normBijection(normBijection_)
				, candidateFunctor(candidateFunctor_)
				, timeIntervalSequence(timeIntervalSequence_)
				, locator(kdTree_.locator())
				, protectiveFactor(normBijection.scalingFactor(1.01))
				, cullDistance(maxDistance_)
				, errorFactor(inverse(normBijection.scalingFactor(1 + maxRelativeError)))
 				, nodeCullDistance(cullDistance * errorFactor)
				, dimension(kdTree_.n())
				, bucketSize(bucketSize_)
				, algorithm()
			{
			}

			void work()
			{
				if (kdTree.empty())
				{
					// The tree does not contain
					// any points.
					return;
				}

				// Note: we consider the search ball open.
				if (maxDistance == 0)
				{
					// The search ball is the empty set.
					return;
				}

				Real rootDistance = 
					distance2(kdTree.bound(), searchPoint, normBijection);

				if (rootDistance >= maxDistance)
				{
					// The bounding box for the points does not
					// intersect the search ball.
					return;
				}

				IndexSequence indexSequence(timeIntervalSequence);
				for (integer i = 0;i < timeIntervalSequence.size();i += 2)
				{
					indexSequence[i] = kdTree.timeToIndex(
						timeIntervalSequence[i]);
					
					if (i + 1 < timeIntervalSequence.size())
					{
						indexSequence[i + 1] = kdTree.timeToIndex(
							timeIntervalSequence[i + 1]);
					}
				}

				// Start from the root node.
				algorithm.insertNode(
					State(kdTree.root(), rootDistance, indexSequence));

				while (algorithm.nodesLeft())
				{
					State state = algorithm.nextNode();


					const Real& distance = state.distance;
					const Cursor& cursor = state.cursor;
					const IndexSequence& intervalSequence = state.indexSequence;

					if (distance > nodeCullDistance)
					{
						if (algorithm.breakOnCulling())
						{
							break;
						}
						else
						{
							continue;
						}
					}

					if (cursor.leaf() ||
						algorithm.shouldSearchSplitNode(state, bucketSize))
					{
						searchBruteForce(state);
					}
					else
					{
						integer splitAxis = cursor.splitAxis();
						Real searchPosition = 
							searchPoint[splitAxis];

						// For an intermediate node our task is to
						// recurse to child nodes while updating
						// incrementally the distance 
						// to the current node. 

						Cursor left = cursor.left();
						Cursor right = cursor.right();

						// Compute the distances to the boundary 
						// planes of the child nodes.

						Real oldAxisDistance = 0;
						Real leftAxisDistance = 0;
						Real rightAxisDistance = 0;
						if (searchPosition < right.min())
						{
							rightAxisDistance = 
								normBijection.axis(right.min() - searchPosition);
							if (searchPosition < left.min())
							{
								leftAxisDistance = 
									normBijection.axis(left.min() - searchPosition);
								if (searchPosition < left.prevMin())
								{
									oldAxisDistance = 
										normBijection.axis(left.prevMin() - searchPosition);
								}
							}
						}
						if (searchPosition > left.max())
						{
							leftAxisDistance = 
								normBijection.axis(searchPosition - left.max());
							if (searchPosition > right.max())
							{
								rightAxisDistance = 
									normBijection.axis(searchPosition - right.max());
								if (searchPosition > right.prevMax())
								{
									oldAxisDistance = 
										normBijection.axis(searchPosition - right.prevMax());
								}
							}
						}
						
						// Compute the actual distances
						// to the child nodes. These are
						// only done if the nodes can't be
						// culled by the boundary plane distance.

						Real leftDistance = leftAxisDistance;
						if (leftAxisDistance <= nodeCullDistance)
						{
							leftDistance = 
								normBijection.replaceAxis(
								distance,
								oldAxisDistance,
								leftAxisDistance);
						}

						Real rightDistance = rightAxisDistance; 
						if (rightAxisDistance <= nodeCullDistance)
						{
							rightDistance = 
								normBijection.replaceAxis(
								distance,
								oldAxisDistance,
								rightAxisDistance);
						}

						// Queue non-culled child nodes for 
						// future handling.

						IndexSequence leftSequence(intervalSequence);
						IndexSequence rightSequence(intervalSequence);
						for (integer i = 0;i < intervalSequence.size();++i)
						{
							leftSequence[i] = cursor.cascade(intervalSequence[i], false);
							rightSequence[i] = cursor.cascade(intervalSequence[i], true);
						}

						State leftState(left, leftDistance, leftSequence);
						State rightState(right, rightDistance, rightSequence);

						if (leftDistance <= nodeCullDistance &&
							!algorithm.skipNode(leftState))
						{
							if (rightDistance <= nodeCullDistance && 
								!algorithm.skipNode(rightState))
							{
								algorithm.insertNodes(
									leftState,
									rightState);
							}
							else
							{
								algorithm.insertNode(leftState);
							}
						}
						else if (rightDistance <= nodeCullDistance && 
							!algorithm.skipNode(rightState))
						{
							algorithm.insertNode(rightState);
						}
					}
				}
			}

			void searchBruteForce(const State& state)
            {
                // We are now in a bucket node.
                // Search through the points in this node.

                Cursor cursor = state.cursor;
				const IndexSequence& indexSequence = state.indexSequence;

				for (integer i = 0; i < indexSequence.size(); i += 2)
				{
					integer indexMin = indexSequence[i];
					integer indexMax = (i + 1) < indexSequence.size() ?
						indexSequence[i + 1] : cursor.points();

					auto pointSet = cursor.pointSetAsInput(indexMin, indexMax);

					Real currentDistance = 0;
					while (!pointSet.empty())
					{
						auto keepGoing = [&](const Real& that)
						{
							return that < cullDistance;
						};

						Point_ConstIterator iter = pointSet.get();
						pointSet.pop();

						currentDistance = distance2(
							location(iter->point(), locator),
							searchPoint,
							normBijection,
							keepGoing);

						// Remember that we are using an open search ball.
						// Here it is enforced again.
						if (currentDistance < cullDistance && acceptPoint(iter))
						{
							// Note that if there are multiple points at the same 
							// distance, then the points after the first should _not_
							// be culled away. We attempt to deal with this
							// by expanding the culling radius by a protective factor.
							Real cullSuggestion =
								candidateFunctor(currentDistance, iter) * protectiveFactor;
							if (cullSuggestion < cullDistance)
							{
								cullDistance = cullSuggestion;
								nodeCullDistance = cullDistance * errorFactor;
							}
						}
					}
				}
            }

			const KdTree& kdTree;
			const Vector<Real, N>& searchPoint;
			const typename KdTree::Point_ConstIterator& searchIter;
			const Real& maxDistance;
			const Real& maxRelativeError;
			const Indicator& acceptPoint;
			const NormBijection& normBijection;
			const CandidateFunctor& candidateFunctor;
			const IntervalSequence& timeIntervalSequence;
			const Locator& locator;
			
			Real protectiveFactor;
			Real cullDistance;
			Real errorFactor;
			Real nodeCullDistance;  
			integer dimension;
			integer bucketSize;
			SearchAlgorithm algorithm;
		};

	}

	template <
		typename KdTree,
		typename Indicator, 
		typename NormBijection,
		typename CandidateFunctor, 
		typename SearchAlgorithm_PointKdTree,
		typename Locator,
		typename Real,
		typename IntervalSequence,
		integer N>
	void searchNearestAlgorithm(
		const KdTree& kdTree,
		const NoDeduction<Vector<Real, N>>& searchPoint,
		const NoDeduction<Real>& maxDistance,
		const NoDeduction<Real>& maxRelativeError,
		const Indicator& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor,
		const SearchAlgorithm_PointKdTree& searchAlgorithm,
		const IntervalSequence& timeIntervalSequence)
	{
		using Cursor = typename KdTree::Cursor;

		NearestAlgorithm_::GenericAlgorithm<KdTree, Indicator, 
			NormBijection, CandidateFunctor, SearchAlgorithm_PointKdTree, IntervalSequence>
			genericAlgorithm(
				kdTree, 
				searchPoint, 
				kdTree.end(), 
				maxDistance, 
				maxRelativeError,
				acceptPoint, 
				bucketSize, 
				normBijection, 
				candidateFunctor,
				timeIntervalSequence);

		genericAlgorithm.work();
	}

	template <
		typename KdTree,
		typename Indicator, 
		typename NormBijection,
		typename CandidateFunctor, 
		typename SearchAlgorithm_PointKdTree,
		typename Locator,
		typename Real,
		typename IntervalSequence>
	void searchNearestAlgorithm(
		const KdTree& kdTree,
		const typename KdTree::Point_ConstIterator& searchIter,
		const NoDeduction<Real>& maxDistance,
		const NoDeduction<Real>& maxRelativeError,
		const Indicator& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor,
		const SearchAlgorithm_PointKdTree& searchAlgorithm,
		const IntervalSequence& timeIntervalSequence)
	{
		if (searchIter == kdTree.end())
		{
			return;
		}

		using Fwd = KdTree;
		PASTEL_FWD(Cursor);
		static constexpr integer N = Locator::N;

		Vector<Real, N> searchPoint =
			pointAsVector(location(searchIter->point(), kdTree.locator()));

		NearestAlgorithm_::GenericAlgorithm<KdTree, Indicator, NormBijection, CandidateFunctor, SearchAlgorithm_PointKdTree,
			IntervalSequence>
			genericAlgorithm(
				kdTree, 
				searchPoint, 
				searchIter, 
				maxDistance, 
				maxRelativeError,
				acceptPoint, 
				bucketSize, 
				normBijection, 
				candidateFunctor,
				timeIntervalSequence);

		genericAlgorithm.work();
	}

}

#endif
