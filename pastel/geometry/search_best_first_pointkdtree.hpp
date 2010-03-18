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
				, errorFactor(inverse(normBijection.scalingFactor(1 + maxRelativeError)))
				, nodeCullDistance(maxDistance_)
				, dimension(kdTree_.dimension())
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
					KeyValue<real, Cursor>, 
					std::vector<KeyValue<real, Cursor> >,
					std::greater<KeyValue<real, Cursor> > > nodeQueue;

				if (!kdTree.root().empty())
				{
					nodeQueue.push(
						keyValue(
						distance2(kdTree.bound(), searchPoint, normBijection),
						kdTree.root()));
				}

				while(!nodeQueue.empty())
				{
					const real distance = nodeQueue.top().key();
					Cursor cursor = nodeQueue.top().value();
					ASSERT(!cursor.empty());

					nodeQueue.pop();

					if (distance > nodeCullDistance)
					{
						// This is the closest node there is
						// of all the non-visited nodes.
						// Because it is beyond the current
						// cull distance, we are done.

						break;
					}

					bool brokeOut = false;
					while(!cursor.leaf() && cursor.objects() > kdTree.bucketSize())
					{
						// For an intermediate node our task is to
						// recurse to child nodes while updating
						// incrementally the distance 
						// to the current node. Note we trace
						// distance to an aligned box rather
						// than just a plane.

						const Real searchPosition = 
							searchPoint[cursor.splitAxis()];
						const Real splitPosition =
							cursor.splitPosition();

						Cursor nearBranch;
						Cursor farBranch;
						Real farBoundDistance = 0;

						// Note: it is essential for numerical
						// precision to use comparison of two values
						// here, rather than comparing the result
						// of a subtraction to zero.

						if (searchPosition < splitPosition)
						{
							// The search point is closer to the left branch so follow that.
							farBranch = cursor.right();
							nearBranch = cursor.left();
							farBoundDistance = normBijection.axis(
								splitPosition - searchPosition);
						}
						else
						{
							// The search point is closer to the right branch so follow that.
							farBranch = cursor.left();
							nearBranch = cursor.right();
							farBoundDistance = normBijection.axis(
								searchPosition - splitPosition);
						}

						// Try to cull the farther node off based on the distance 
						// of the search point to the farther bound.
						// The far branch is only searched if it is non-empty.

						if (farBoundDistance <= nodeCullDistance && !farBranch.empty())
						{
							// Try to cull the farther node off based on the distance 
							// of the search point to the farther child node.

							Real oldAxisDistance = 0;
							if (searchPosition < cursor.min())
							{
								oldAxisDistance = 
									normBijection.axis(cursor.min() - searchPosition);
							}
							else if (searchPosition > cursor.max())
							{
								oldAxisDistance = 
									normBijection.axis(searchPosition - cursor.max());
							}

							const Real childDistance = 
								normBijection.replaceAxis(
								distance, 
								oldAxisDistance,
								farBoundDistance);

							if (childDistance <= nodeCullDistance)
							{
								// No culling could be done, place the farther node
								// into the queue.
								nodeQueue.push(keyValue(childDistance, farBranch));
							}
						}

						if (nearBranch.empty())
						{
							// If the near child is empty,
							// then we should get another
							// node from the queue.
							brokeOut = true;
							break;
						}
						else
						{
							// Otherwise we should continue
							// searching the near branch, since
							// it still has the nearest node.
							cursor = nearBranch;
						}
					}
					if (!brokeOut)
					{
						searchBruteForce(cursor);
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
							nodeCullDistance = cullDistance * errorFactor;
						}
						/*
						cullDistance = std::min(
							candidateFunctor.suggestCullDistance() * protectiveFactor,
							cullDistance);
						nodeCullDistance = cullDistance * errorFactor;
						*/
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
			Real errorFactor;
			Real nodeCullDistance;			
			integer dimension;
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
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor)
	{
		if (kdTree.empty())
		{
			return;
		}

		Detail_BestFirst::BestFirst<Real, N, ObjectPolicy, AcceptPoint, NormBijection, CandidateFunctor>
			bestFirst(kdTree, searchPoint, maxDistance, maxRelativeError,
			acceptPoint, normBijection, candidateFunctor);

		bestFirst.work();
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename AcceptPoint, typename NormBijection, 
		typename CandidateFunctor>
	void searchBestFirst(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor)
	{
		if (kdTree.empty())
		{
			return;
		}

		Vector<Real, N> searchPoint2(
			ofDimension(kdTree.dimension()), 
			withAliasing((Real*)kdTree.objectPolicy().point(searchPoint->object())));

		Detail_BestFirst::BestFirst<Real, N, ObjectPolicy, AcceptPoint, NormBijection, CandidateFunctor>
			bestFirst(kdTree, searchPoint2, maxDistance, maxRelativeError,
			acceptPoint, normBijection, candidateFunctor);

		bestFirst.work();
	}

}

#endif
