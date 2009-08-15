#ifndef PASTEL_SEARCH_DEPTH_FIRST_POINTKDTREE_HPP
#define PASTEL_SEARCH_DEPTH_FIRST_POINTKDTREE_HPP

#include "pastel/geometry/search_depth_first_pointkdtree.h"
#include "pastel/geometry/distance_alignedbox_point.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Detail_DepthFirst
	{

		// Depth-first approximate search

		template <typename Real, int N, typename ObjectPolicy, 
			typename NormBijection, typename CandidateFunctor>
		class DepthFirst
		{
		private:
			typedef PointKdTree<Real, N, ObjectPolicy> Tree;
			typedef typename Tree::Cursor Cursor;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;

		public:
			DepthFirst(
				const PointKdTree<Real, N, ObjectPolicy>& kdTree_,
				const Point<Real, N>& searchPoint_,
				const Real& maxDistance_,
				const Real& maxRelativeError_,
				const NormBijection& normBijection_,
				const CandidateFunctor& candidateFunctor_)
				: kdTree(kdTree_)
				, searchPoint(searchPoint_)
				, maxDistance(maxDistance_)
				, maxRelativeError(maxRelativeError_)
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

			void workTopDown()
			{
				workTopDown(kdTree.root(), 
					distance2(kdTree.bound(), searchPoint, normBijection));
			}

			void workBottomUp(const Cursor& startCursor)
			{
				ENSURE(startCursor.isBucket());

				if (!startCursor.empty())
				{
					workTopDown(startCursor, 0);
				}

				Cursor previous = startCursor;
				Cursor cursor = startCursor.parent();
				while(cursor.exists())
				{
					const Real searchPosition = 
						searchPoint[cursor.splitAxis()];
					const Real splitPosition =
						cursor.splitPosition();

					Cursor farBranch;
					Real farBoundDistance = 0;

					if (previous == cursor.left())
					{
						farBranch = cursor.right();
						if (searchPosition < splitPosition)
						{
							farBoundDistance = normBijection.axis(
								splitPosition - searchPosition);
						}
					}
					else
					{
						farBranch = cursor.left();
						if (searchPosition > splitPosition)
						{
							farBoundDistance = normBijection.axis(
								searchPosition - splitPosition);
						}
					}

					// Try to cull the farther node off based on the distance 
					// of the search point to the farther bound.
					if (!farBranch.empty() &&
						farBoundDistance <= nodeCullDistance)
					{
						// No culling could be done, visit the farther node
						// recursively.
						workTopDown(farBranch, farBoundDistance);
					}
					
					previous = cursor;
					cursor = cursor.parent();
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
						Point<Real, N>(ofDimension(dimension), 
						withAliasing((Real*)objectPolicy.point(iter->object()))), 
						searchPoint, 
						normBijection, cullDistance);

					// It is essential that this is <= rather
					// than <, because of the possibility
					// of multiple points at same location.
					if (currentDistance <= cullDistance)
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

			void workTopDown(const Cursor& cursor, const Real& distance)
			{
				if (cursor.isBucket())
				{
					searchBruteForce(cursor);
				}
				else
				{
					ASSERT(!cursor.leaf());

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
						if (searchPosition < splitPosition)
						{
							farBoundDistance = normBijection.axis(
								splitPosition - searchPosition);
						}
					}
					else
					{
						// The search point is closer to the right branch so follow that.
						farBranch = cursor.left();
						nearBranch = cursor.right();
						if (searchPosition > splitPosition)
						{
							farBoundDistance = normBijection.axis(
								searchPosition - splitPosition);
						}
					}

					// Follow downwards the kdTree with the nearer node.
					workTopDown(nearBranch, distance);

					// Try to cull the farther node off based on the distance 
					// of the search point to the farther bound.

					if (farBoundDistance <= nodeCullDistance)
					{
						// Try to cull the farther node off based on the distance 
						// of the search point to the farther child node.

						Real oldAxisDistance = 0;
						if (searchPosition < cursor.min())
						{
							oldAxisDistance = cursor.min() - searchPosition;
						}
						else if (searchPosition > cursor.max())
						{
							oldAxisDistance = searchPosition - cursor.max();
						}

						const Real childDistance = 
							normBijection.replaceAxis(
							distance, 
							normBijection.axis(oldAxisDistance),
							farBoundDistance);

						if (childDistance <= nodeCullDistance)
						{
							// No culling could be done, visit the farther node.
							workTopDown(farBranch, childDistance);
						}
					}
				}
			}

			const PointKdTree<Real, N, ObjectPolicy>& kdTree;
			const Point<Real, N>& searchPoint;
			const Real& maxDistance;
			const Real& maxRelativeError;
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

	template <int N, typename Real, typename ObjectPolicy, 
		typename NormBijection, typename CandidateFunctor>
	void searchDepthFirst(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const Point<Real, N>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor)
	{
		if (kdTree.empty())
		{
			return;
		}

		Detail_DepthFirst::DepthFirst<Real, N, ObjectPolicy, NormBijection, CandidateFunctor>
			depthFirst(kdTree, searchPoint, maxDistance, maxRelativeError,
			normBijection, candidateFunctor);

		depthFirst.workTopDown();
	}

	template <int N, typename Real, typename ObjectPolicy, 
		typename NormBijection, typename CandidateFunctor>
	void searchDepthFirst(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		const CandidateFunctor& candidateFunctor)
	{
		if (kdTree.empty())
		{
			return;
		}

		Point<Real, N> searchPoint2(
			ofDimension(kdTree.dimension()), 
			withAliasing((Real*)kdTree.objectPolicy().point(searchPoint->object())));

		Detail_DepthFirst::DepthFirst<Real, N, ObjectPolicy, NormBijection, CandidateFunctor>
			depthFirst(kdTree, searchPoint2, 
			maxDistance, maxRelativeError,
			normBijection, candidateFunctor);

		depthFirst.workBottomUp(searchPoint->bucket());
	}

}

#endif
