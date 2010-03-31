#ifndef PASTEL_SEARCH_NEAREST_ALGORITHM_POINTKDTREE_HPP
#define PASTEL_SEARCH_NEAREST_ALGORITHM_POINTKDTREE_HPP

#include "pastel/geometry/search_nearest_algorithm_pointkdtree.h"
#include "pastel/geometry/distance_alignedbox_point.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/keyvalue.h"

#include <queue>

namespace Pastel
{

	namespace Detail_NearestAlgorithm
	{

		template <typename Real, int N, typename ObjectPolicy, 
			typename AcceptPoint, typename NormBijection, 
			typename CandidateFunctor, typename SearchAlgorithm>
		class GenericAlgorithm
		{
		private:
			typedef PointKdTree<Real, N, ObjectPolicy> Tree;
			typedef typename Tree::Cursor Cursor;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;

		public:
			GenericAlgorithm(
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
				, algorithm()
			{
			}

			void work()
			{
				if (kdTree.empty())
				{
					// The tree does not contain
					// any objects.
					return;
				}

				// Note: we consider the search ball open.
				if (maxDistance == 0)
				{
					// The search ball is the empty set.
					return;
				}

				const Real rootDistance = 
					distance2(kdTree.bound(), searchPoint, normBijection);

				if (rootDistance >= maxDistance)
				{
					// The bounding box for the points does not
					// intersect the search ball.
					return;
				}

				// Start from the root node.
				algorithm.insertNode(kdTree.root(), rootDistance);

				while(true)
				{
					const KeyValue<Real, Cursor> entry =
						algorithm.nextNode();

					const Real& distance = entry.key();
					const Cursor& cursor = entry.value();

					if (!cursor.exists())
					{
						break;
					}
					
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
						algorithm.shouldSearchSplitNode(cursor, bucketSize))
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

						if (leftDistance <= nodeCullDistance && 
							!algorithm.skipNode(left))
						{
							if (rightDistance <= nodeCullDistance && 
								!algorithm.skipNode(right))
							{
								algorithm.insertNodes(
									left, leftDistance,
									right, rightDistance);
							}
							else
							{
								algorithm.insertNode(left, leftDistance);
							}
						}
						else if (rightDistance <= nodeCullDistance && 
							!algorithm.skipNode(right))
						{
							algorithm.insertNode(right, rightDistance);
						}
					}
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

					// Remember that we are using an open search ball.
					// Here it is enforced again.
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
			SearchAlgorithm algorithm;
		};

	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename AcceptPoint, typename NormBijection, 
		typename CandidateFunctor, typename SearchAlgorithm_PointKdTree>
	void searchNearestAlgorithm(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const Vector<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor,
		const SearchAlgorithm_PointKdTree& searchAlgorithm)
	{
		typedef typename PointKdTree<Real, N, ObjectPolicy>::Cursor Cursor;
		typedef typename SearchAlgorithm_PointKdTree::Instance<Real, Cursor> SearchAlgorithm;

		Detail_NearestAlgorithm::GenericAlgorithm<Real, N, ObjectPolicy, AcceptPoint, NormBijection, CandidateFunctor, SearchAlgorithm>
			genericAlgorithm(kdTree, searchPoint, kdTree.end(), maxDistance, maxRelativeError,
			acceptPoint, bucketSize, normBijection, candidateFunctor);

		genericAlgorithm.work();
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename AcceptPoint, typename NormBijection, 
		typename CandidateFunctor, typename SearchAlgorithm_PointKdTree>
	void searchNearestAlgorithm(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator& searchIter,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor,
		const SearchAlgorithm_PointKdTree& searchAlgorithm)
	{
		if (searchIter == kdTree.end())
		{
			return;
		}

		typedef typename PointKdTree<Real, N, ObjectPolicy>::Cursor Cursor;
		typedef typename SearchAlgorithm_PointKdTree::Instance<Real, Cursor> SearchAlgorithm;

		const Vector<Real, N> searchPoint(
			ofDimension(kdTree.dimension()),
			withAliasing((Real*)kdTree.objectPolicy().point(searchIter->object())));

		Detail_NearestAlgorithm::GenericAlgorithm<Real, N, ObjectPolicy, AcceptPoint, NormBijection, CandidateFunctor, SearchAlgorithm>
			genericAlgorithm(kdTree, searchPoint, searchIter, maxDistance, maxRelativeError,
			acceptPoint, bucketSize, normBijection, candidateFunctor);

		genericAlgorithm.work();
	}

}

#endif
