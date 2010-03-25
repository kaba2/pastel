#ifndef PASTEL_SEARCH_BEST_FIRST_POINTKDTREE_HPP
#define PASTEL_SEARCH_BEST_FIRST_POINTKDTREE_HPP

#include "pastel/geometry/search_best_first_pointkdtree.h"
#include "pastel/geometry/distance_alignedbox_point.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/keyvalue.h"

#include <queue>

namespace Pastel
{

	namespace Detail_BestFirst
	{

		// Best-first approximate search

		template <typename Real, int N, typename ObjectPolicy, 
			typename AcceptPoint, typename NormBijection, 
			typename CandidateFunctor>
		class BestFirst
		{
		private:
			typedef PointKdTree<Real, N, ObjectPolicy> Tree;
			typedef typename Tree::Cursor Cursor;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;

		public:
			BestFirst(
				const PointKdTree<Real, N, ObjectPolicy>& kdTree_,
				const Vector<Real, N>& searchPoint_,
				const Real& maxDistance_,
				const Real& maxRelativeError_,
				const AcceptPoint& acceptPoint_,
				integer bucketSize_,
				const NormBijection& normBijection_,
				const CandidateFunctor& candidateFunctor_)
				: kdTree(kdTree_)
				, searchPoint(searchPoint_)
				, maxDistance(maxDistance_)
				, maxRelativeError(maxRelativeError_)
				, acceptPoint(acceptPoint_)
				, normBijection(normBijection_)
				, candidateFunctor(candidateFunctor_)
				, objectPolicy(kdTree_.objectPolicy())
				, protectiveFactor(normBijection.scalingFactor(1.001))
				, cullDistance(maxDistance_)
				, dimension(kdTree_.dimension())
				, bucketSize(bucketSize_)
			{
				// Due to rounding errors exact comparisons can miss
				// reporting some of the points, giving incorrect results.
				// For example, consider n > k points on a 2d circle and make a 
				// k-nearest query to its center. With bad luck the algorithm
				// can report less than k points. We avoid this behaviour
				// by scaling the culling radius up by a protective factor.
				// However, when doing this one must not allow for points
				// that have distance more than maxDistance to creep
				// into results.
			}

			void work()
			{
				std::priority_queue<
					KeyValue<Real, Cursor>, 
					std::vector<KeyValue<Real, Cursor> >,
					std::greater<KeyValue<Real, Cursor> > > nodeQueue;

				if (!kdTree.root().empty())
				{
					nodeQueue.push(
						keyValue((Real)0, kdTree.root()));
				}

				while(!nodeQueue.empty())
				{
					const Real distance = nodeQueue.top().key();
					Cursor cursor = nodeQueue.top().value();
					ASSERT(!cursor.empty());

					nodeQueue.pop();

					if (distance > cullDistance)
					{
						// This is the closest node there is
						// of all the non-visited nodes.
						// Because it is beyond the current
						// cull distance, we are done.

						break;
					}
					
					if (cursor.leaf() || cursor.objects() <= bucketSize)
					{
						searchBruteForce(cursor);
					}
					else
					{
						// For an intermediate node our task is to
						// recurse to child nodes while updating
						// incrementally the distance 
						// to the current node. Note we trace
						// distance to an aligned box rather
						// than just a plane.

						const Real searchPosition = 
							searchPoint[cursor.splitAxis()];

						const Cursor left = cursor.left();
						const Cursor right = cursor.right();

						const Real leftMin = left.min();
						const Real leftMax = left.max();
						const Real rightMin = right.min();
						const Real rightMax = right.max();

						Real leftDistance = distance;
						Real rightDistance = distance;

						// Note: it is essential for numerical
						// precision to use comparisons of values
						// here, rather than comparing the result
						// of a subtraction to zero.

						if (searchPosition < leftMin)
						{
							leftDistance = normBijection.addAxis(
								distance,
								normBijection.axis(leftMin - searchPosition));
							rightDistance = normBijection.addAxis(
								distance,
								normBijection.axis(rightMin - searchPosition));
							
							// Since the distance to the left child does
							// not change, it must be the closest of all currently
							// considered nodes: continue with that.
							cursor = left;
							nodeQueue.push(keyValue(rightDistance, right));
						}
						else if (searchPosition > rightMax)
						{
							leftDistance = normBijection.addAxis(
								distance,
								normBijection.axis(searchPosition - leftMax));
							rightDistance = normBijection.addAxis(
								distance,
								normBijection.axis(searchPosition - rightMax));

							// Since the distance to the right child does
							// not change, it must be the closest of all currently
							// considered nodes: continue with that.
							cursor = right;
						}
						else 
						{
							if (searchPosition > leftMax)
							{
								leftDistance = normBijection.addAxis(
									distance,
									normBijection.axis(searchPosition - leftMax));
								nodeQueue.push(keyValue(leftDistance, left));
							}
							if (searchPosition < rightMin)
							{
								rightDistance = normBijection.addAxis(
									distance,
									normBijection.axis(rightMin - searchPosition));
							}
						}
						nodeQueue.push(keyValue(leftDistance, left));
						nodeQueue.push(keyValue(rightDistance, right));
					}
				}
			}

		private:
			void searchBruteForce(const Cursor& cursor)
			{
				// We are now in a bucket node.
				// Search through the objects in this node.

				ConstObjectIterator iter = cursor.begin();
				const ConstObjectIterator iterEnd = cursor.end();

				while(iter != iterEnd)
				{
					const Real currentDistance = 
						distance2(
						Vector<Real, N>(ofDimension(dimension), 
						withAliasing((Real*)objectPolicy.point(iter->object()))), 
						searchPoint, 
						normBijection, cullDistance);

					// It is essential that this is <= rather
					// than <, because of the possibility
					// of multiple points at same location.
					if (currentDistance <= cullDistance && acceptPoint(iter))
					{
						candidateFunctor(currentDistance, iter);
						const Real cullSuggestion = 
							candidateFunctor.suggestCullDistance() * protectiveFactor;
						if (cullSuggestion < cullDistance)
						{
							cullDistance = cullSuggestion;
						}
					}

					++iter;
				}
			}

			const Tree& kdTree;
			const Vector<Real, N>& searchPoint;
			const Real& maxDistance;
			const Real& maxRelativeError;
			const AcceptPoint& acceptPoint;
			const NormBijection& normBijection;
			const CandidateFunctor& candidateFunctor;
			const ObjectPolicy& objectPolicy;
			
			Real protectiveFactor;
			Real cullDistance;
			integer dimension;
			integer bucketSize;
		};

	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename AcceptPoint, typename NormBijection, 
		typename CandidateFunctor>
	void searchBestFirst(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor)
	{
		if (kdTree.empty())
		{
			return;
		}

		Detail_BestFirst::BestFirst<Real, N, ObjectPolicy, AcceptPoint, NormBijection, CandidateFunctor>
			bestFirst(kdTree, searchPoint, maxDistance, maxRelativeError,
			acceptPoint, bucketSize, normBijection, candidateFunctor);

		bestFirst.work();
	}

}

#endif
