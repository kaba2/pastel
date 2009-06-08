#ifndef PASTEL_POINTKDTREE_DEPTH_FIRST_HPP
#define PASTEL_POINTKDTREE_DEPTH_FIRST_HPP

#include "pastel/geometry/pointkdtree_depth_first.h"
#include "pastel/geometry/distance_alignedbox_point.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	// Depth-first approximate search

	template <int N, typename Real, typename ObjectPolicy, 
		typename NormBijection, typename CandidateFunctor>
	class DepthFirst
	{
	private:
		typedef PointKdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;

	public:
		DepthFirst(
			const PointKdTree<N, Real, ObjectPolicy>& kdTree_,
			const Point<N, Real>& searchPoint_,
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
			work(kdTree.root(), 
				distance2(kdTree.bound(), searchPoint, normBijection));
		}

		void work(const Cursor& cursor, const Real& distance)
		{
			if (distance > nodeCullDistance)
			{
				// The node is beyond cull distance, skip it.
				return;
			}

			if (!cursor.leaf())
			{
				// For an intermediate node our task is to
				// recurse to child nodes while updating
				// incrementally the distance 
				// to the  current node. This distance
				// is exact for the kdtree nodes, but
				// only approximate for arbitrary
				// directed splitting planes (note
				// however that also in this case we trace 
				// the distance to rectangular nodes rather than
				// just to planes).

				const Real searchPosition = 
					searchPoint[cursor.splitAxis()];

				Cursor nearBranch;
				Cursor farBranch;
				Real farBoundDistance = 0;

				// Note: it is essential for numerical
				// precision to use comparison of two values
				// here, rather than comparing the result
				// of a subtraction to zero.

				if (cursor.projectedPosition(searchPoint) < 
					cursor.splitPosition())
				{
					// The search point is closer to the left branch so follow that.
					farBranch = cursor.positive();
					nearBranch = cursor.negative();
					if (searchPosition < cursor.positiveMin())
					{
						farBoundDistance = normBijection.axis(
							cursor.positiveMin() - searchPosition);
					}
				}
				else
				{
					// The search point is closer to the right branch so follow that.
					farBranch = cursor.negative();
					nearBranch = cursor.positive();
					if (searchPosition > cursor.negativeMax())
					{
						farBoundDistance = normBijection.axis(
							searchPosition - cursor.negativeMax());
					}
				}

				// Follow downwards the kdTree with the nearer node.
				work(nearBranch, distance);

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
						// No culling could be done, visit the farther node
						// later.
						work(farBranch, childDistance);
					}
				}
			}
			else
			{
				// We are now in a leaf node.
				// Search through the objects in this node.

				ConstObjectIterator iter = cursor.begin();
				ConstObjectIterator iterEnd = cursor.end();

				while(iter != iterEnd)
				{
					const Real currentDistance = 
						distance2(objectPolicy.point(*iter), searchPoint, 
						normBijection, cullDistance);

					// It is essential that this is <= rather
					// than <, because of the possibility
					// of multiple points at same location.
					if (currentDistance <= cullDistance)
					{
						const Real cullDistanceSuggestion = 
							candidateFunctor(currentDistance, cullDistance, iter);
						
						cullDistance = std::min(
							cullDistanceSuggestion * protectiveFactor,
							cullDistance);
						nodeCullDistance = cullDistance * errorFactor;
					}

					++iter;
				}
			}
		}

	private:
		const PointKdTree<N, Real, ObjectPolicy>& kdTree;
		const Point<N, Real>& searchPoint;
		const Real& maxDistance;
		const Real& maxRelativeError;
		const NormBijection& normBijection;
		const CandidateFunctor& candidateFunctor;
		const ObjectPolicy& objectPolicy;
		
		Real protectiveFactor;
		Real cullDistance;
		Real errorFactor;
		Real nodeCullDistance;
	};

}

#endif
