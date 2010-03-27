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
				const typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator& searchIter_,
				const Real& maxDistance_,
				const Real& maxRelativeError_,
				const AcceptPoint& acceptPoint_,
				integer bucketSize_,
				const NormBijection& normBijection_,
				const CandidateFunctor& candidateFunctor_)
				: kdTree(kdTree_)
				, searchPoint(searchPoint_)
				, searchIter(searchIter_)
				, maxDistance(maxDistance_)
				, maxRelativeError(maxRelativeError_)
				, acceptPoint(acceptPoint_)
				, normBijection(normBijection_)
				, candidateFunctor(candidateFunctor_)
				, objectPolicy(kdTree_.objectPolicy())
				, protectiveFactor(normBijection.scalingFactor(1.001))
				, cullDistance(maxDistance_)
				, errorFactor(inverse(normBijection.scalingFactor(1 + maxRelativeError)))
 				, nodeCullDistance(maxDistance_)
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
				typedef KeyValue<Real, Cursor> Entry;
				typedef std::priority_queue<Entry,
					std::vector<Entry>,
					std::greater<Entry> > EntrySet;

				Cursor initialNode; 
				if (searchIter != kdTree.end())
				{
					// If the search point is one of the points
					// in the tree, we can use this information
					// for better performance by scanning its
					// leaf node at once. This is useful especially
					// in k-nearest neighbor searching in higher 
					// dimensions where the distances to points provide 
					// pruning. 

					initialNode = searchIter->leaf();
					while(initialNode.parent().exists() && 
						initialNode.parent().objects() <= bucketSize)
					{
						initialNode = initialNode.parent();
					}
					searchBruteForce(initialNode);
				}

				EntrySet nodeQueue;

				if (!kdTree.root().empty())
				{
					nodeQueue.push(
						keyValue(distance2(kdTree.bound(), searchPoint, normBijection), 
						kdTree.root()));
				}

				while(!nodeQueue.empty())
				{
					Real distance = nodeQueue.top().key();
					Cursor cursor = nodeQueue.top().value();
					nodeQueue.pop();

					if (cursor == initialNode)
					{
						continue;
					}

					if (distance > nodeCullDistance)
					{
						// This is the closest node and it is
						// beyond the maximum distance. We are
						// done.
						break;
					}

					if (cursor.leaf() || cursor.objects() <= bucketSize)
					{
						searchBruteForce(cursor);
					}
					else
					{
						const integer splitAxis = cursor.splitAxis();
						const Real searchPosition = 
							searchPoint[splitAxis];

						// For an intermediate node our task is to
						// recurse to child nodes while updating
						// incrementally the distance 
						// to the current node. 

						const Cursor left = cursor.left();
						const Cursor right = cursor.right();

						// First compute the distances to splitting planes.

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
						bool visitLeft = false;
						Real leftDistance = 0;
						if (leftAxisDistance <= nodeCullDistance)
						{
							// The left node can't be culled by the axis
							// distance. Let's see if it can be culled
							// by the actual distance.
							leftDistance = 
								normBijection.replaceAxis(
								distance,
								oldAxisDistance,
								leftAxisDistance);
							if (leftDistance <= nodeCullDistance)
							{
								nodeQueue.push(keyValue(leftDistance, left));
							}
						}

						bool visitRight = false;
						Real rightDistance = 0; 
						if (rightAxisDistance <= nodeCullDistance)
						{
							// The right node can't be culled by the axis
							// distance. Let's see if it can be culled
							// by the actual distance.
							rightDistance = 
								normBijection.replaceAxis(
								distance,
								oldAxisDistance,
								rightAxisDistance);
							if (rightDistance <= nodeCullDistance)
							{
								nodeQueue.push(keyValue(rightDistance, right));
							}
						}
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
                        objectPolicy.point(iter->object()),
                        searchPoint.rawBegin(),
						dimension,
                        normBijection, 
						cullDistance);

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
							nodeCullDistance = cullDistance * errorFactor;
                        }
                    }

                    ++iter;
                }
            }

			const Tree& kdTree;
			const Vector<Real, N>& searchPoint;
			const typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator& searchIter;
			const Real& maxDistance;
			const Real& maxRelativeError;
			const AcceptPoint& acceptPoint;
			const NormBijection& normBijection;
			const CandidateFunctor& candidateFunctor;
			const ObjectPolicy& objectPolicy;
			
			Real protectiveFactor;
			Real cullDistance;
			Real errorFactor;
			Real nodeCullDistance;  
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
			bestFirst(kdTree, searchPoint, kdTree.end(), maxDistance, maxRelativeError,
			acceptPoint, bucketSize, normBijection, candidateFunctor);

		bestFirst.work();
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename AcceptPoint, typename NormBijection, 
		typename CandidateFunctor>
	void searchBestFirst(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator& searchIter,
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
			bestFirst(kdTree, kdTree.point(searchIter->object()), searchIter, maxDistance, maxRelativeError,
			acceptPoint, bucketSize, normBijection, candidateFunctor);

		bestFirst.work();
	}

}

#endif
