#ifndef PASTEL_SEARCH_DEPTH_FIRST_POINTKDTREE_HPP
#define PASTEL_SEARCH_DEPTH_FIRST_POINTKDTREE_HPP

#include "pastel/geometry/search_depth_first_pointkdtree.h"
#include "pastel/geometry/distance_alignedbox_point.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/keyvalue.h"

#include <queue>

namespace Pastel
{

	namespace Detail_DepthFirst
	{

		// Depth-first approximate search

		template <typename Real, int N, typename ObjectPolicy, 
			typename AcceptPoint, typename NormBijection, 
			typename CandidateFunctor>
		class DepthFirst
		{
		private:
			typedef PointKdTree<Real, N, ObjectPolicy> Tree;
			typedef typename Tree::Cursor Cursor;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;

		public:
			DepthFirst(
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
				, protectiveFactor(normBijection.scalingFactor(1.01))
				, cullDistance(maxDistance_)
				, errorFactor(inverse(normBijection.scalingFactor(1 + maxRelativeError)))
 				, nodeCullDistance(cullDistance * errorFactor)
				, dimension(kdTree_.dimension())
				, bucketSize(bucketSize_)
			{
			}

			void work()
			{
				if (maxDistance == 0)
				{
					return;
				}

				workTopDown(kdTree.root(),
					distance2(kdTree.bound(), searchPoint, normBijection));
			}

		private:
			void workTopDown(Cursor cursor, Real distance)
			{
				while(!cursor.leaf() && cursor.objects() > bucketSize)
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
					if (leftAxisDistance <= nodeCullDistance && !left.empty())
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
							visitLeft = true;
						}
					}

					bool visitRight = false;
					Real rightDistance = 0; 
					if (rightAxisDistance <= nodeCullDistance && !right.empty())
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
							visitRight = true;
						}
					}

					if (visitLeft)
					{
						if (visitRight)
						{
							if (leftDistance < rightDistance)
							{
								workTopDown(left, leftDistance);
								cursor = right;
								distance = rightDistance;
							}
							else
							{
								workTopDown(right, rightDistance);
								cursor = left;
								distance = leftDistance;
							}
						}
						else
						{
							cursor = left;
							distance = leftDistance;
						}
					}
					else
					{
						if (visitRight)
						{
							cursor = right;
							distance = rightDistance;
						}
						else
						{
							break;
						}
					}
				}
				if (cursor.leaf() || cursor.objects() <= bucketSize)
				{
					searchBruteForce(cursor);
				}
			}

			void searchBruteForce(const Cursor& cursor)
            {
                // We are now in a bucket node.
                // Search through the objects in this node.

                ConstObjectIterator iter = cursor.begin();
                const ConstObjectIterator iterEnd = cursor.end();

				Real currentDistance = 0;
                while(iter != iterEnd)
                {
                    currentDistance = 
                        distance2(
                        objectPolicy.point(iter->object()),
                        searchPoint.rawBegin(),
						dimension,
                        normBijection, 
						cullDistance);

					// In the case of a fixed-radius search, we
					// use an open search ball. 
                    if (currentDistance < cullDistance && acceptPoint(iter))
                    {
                        candidateFunctor(currentDistance, iter);
						// Note that if there are multiple points at the same 
						// distance, then the points after the first should _not_
						// be culled away. We attempt to deal with this
						// by expanding the culling radius by a protective factor.
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
	void searchDepthFirst(
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

		Detail_DepthFirst::DepthFirst<Real, N, ObjectPolicy, AcceptPoint, NormBijection, CandidateFunctor>
			depthFirst(kdTree, searchPoint, kdTree.end(), maxDistance, maxRelativeError,
			acceptPoint, bucketSize, normBijection, candidateFunctor);

		depthFirst.work();
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename AcceptPoint, typename NormBijection, 
		typename CandidateFunctor>
	void searchDepthFirst(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator& searchIter,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor)
	{
		if (kdTree.empty() || searchIter == kdTree.end())
		{
			return;
		}

		const Vector<Real, N> searchPoint(
			ofDimension(kdTree.dimension()),
			withAliasing((Real*)kdTree.objectPolicy().point(searchIter->object())));

		Detail_DepthFirst::DepthFirst<Real, N, ObjectPolicy, AcceptPoint, NormBijection, CandidateFunctor>
			depthFirst(kdTree, searchPoint, searchIter, maxDistance, maxRelativeError,
			acceptPoint, bucketSize, normBijection, candidateFunctor);

		depthFirst.work();
	}

}

#endif
