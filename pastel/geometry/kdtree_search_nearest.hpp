#ifndef PASTELGEOMETRY_KDTREE_SEARCH_NEAREST_HPP
#define PASTELGEOMETRY_KDTREE_SEARCH_NEAREST_HPP

#include "pastel/geometry/kdtree_search_nearest.h"
#include "pastel/geometry/distance_alignedbox_point.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/smallfixedset.h"

#include <set>
#include <vector>

namespace Pastel
{

	// Depth-first approximate search

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	class SearchNearest
	{
	private:
		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;

	public:
		SearchNearest(
			const KdTree<N, Real, ObjectPolicy>& kdTree_,
			const Point<N, Real>& searchPoint_,
			const Real& maxDistance_,
			const Real& maxRelativeError_,
			const NormBijection& normBijection_,
			integer kNearest_,
			std::vector<typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>* nearestSet_,
			std::vector<Real>* distanceSet_)
			: kdTree(kdTree_)
			, searchPoint(searchPoint_)
			, maxDistance(maxDistance_)
			, maxRelativeError(maxRelativeError_)
			, normBijection(normBijection_)
			, kNearest(kNearest_)
			, nearestSet(nearestSet_)
			, distanceSet(distanceSet_)
			, objectPolicy(kdTree_.objectPolicy())
			, protectiveFactor(normBijection.scalingFactor(1.001))
			, cullDistance(maxDistance_)
			, errorFactor(inverse(normBijection.scalingFactor(1 + maxRelativeError)))
			, nodeCullDistance(maxDistance_)
			, candidateSet(kNearest_)
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
			if (kdTree.empty() || kNearest == 0 ||
				(!nearestSet && !distanceSet))
			{
				return;
			}

			work(kdTree.root(), 
				distance2(kdTree.bound(), searchPoint, normBijection));

			const integer foundNearest = candidateSet.size();

			if (nearestSet)
			{
				CandidateIterator iter = candidateSet.begin();
				const CandidateIterator iterEnd = candidateSet.end();
				integer i = 0;
				while(iter != iterEnd)
				{
					(*nearestSet)[i] = iter->value();
					++iter;
					++i;
				}
			}

			if (distanceSet)
			{
				CandidateIterator iter = candidateSet.begin();
				const CandidateIterator iterEnd = candidateSet.end();
				integer i = 0;
				while(iter != iterEnd)
				{
					(*distanceSet)[i] = iter->key();
					++iter;
				}
			}
		}

		void work(const Cursor& cursor, const Real& distance)
		{
			if (distance > nodeCullDistance)
			{
				// The node is beyond cull distance, skip it.
				return;
			}

			// Find the leaf node that is closest
			// to the search searchPoint in this current branch.
			// Place all the alternative branching nodes into
			// a nodeStack.
			if (!cursor.leaf())
			{
				const integer splitAxis = cursor.splitAxis();

				const Real searchPosition =
					searchPoint[splitAxis];

				const Real splitPosition = 
					cursor.splitPosition();

				Cursor nearBranch;
				Cursor farBranch;

				if (searchPosition < splitPosition)
				{
					// The search point is closer to the left branch so follow that.
					farBranch = cursor.positive();
					nearBranch = cursor.negative();
				}
				else
				{
					// The search point is closer to the right branch so follow that.
					farBranch = cursor.negative();
					nearBranch = cursor.positive();
				}

				// Follow downwards the kdTree with the nearer node.
				work(nearBranch, distance);

				// Try to cull the farther node off based on the distance 
				// of the search point to the splitting plane.
				const Real planeDistance = normBijection.signedAxis(
					splitPosition - searchPosition);

				if (planeDistance <= nodeCullDistance)
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
						planeDistance);

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
					const real currentDistance = 
						distance2(objectPolicy.point(*iter), searchPoint, 
						normBijection, cullDistance);

					// It is essential that this is <= rather
					// than <, because of the possibility
					// of multiple points at same location.
					if (currentDistance <= cullDistance)
					{
						candidateSet.insert(
							KeyVal(currentDistance, iter));

						if (candidateSet.full())
						{
							cullDistance = std::min(
								candidateSet.back().key() * protectiveFactor,
								maxDistance);
							nodeCullDistance = cullDistance * errorFactor;
						}
					}

					++iter;
				}
			}
		}

	private:
		class NodeEntry
		{
		public:
			NodeEntry(
				const typename Cursor& node,
				const Real& distance)
				: node_(node)
				, distance_(distance)
			{
			}

			bool operator<(const NodeEntry& that) const
			{
				if (distance_ < that.distance_)
				{
					return true;
				}
				if (that.distance_ < distance_)
				{
					return false;
				}

				return node_ < that.node_;
			}

			typename KdTree<N, Real, ObjectPolicy>::Cursor node_;
			Real distance_;
		};

		const KdTree<N, Real, ObjectPolicy>& kdTree;
		const Point<N, Real>& searchPoint;
		const Real& maxDistance;
		const Real& maxRelativeError;
		const NormBijection& normBijection;
		integer kNearest;
		std::vector<typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>* nearestSet;
		std::vector<Real>* distanceSet;
		const ObjectPolicy& objectPolicy;
		
		Real protectiveFactor;
		Real cullDistance;
		Real errorFactor;
		Real nodeCullDistance;

		typedef SmallFixedSet<KeyVal> CandidateSet;
		typedef typename CandidateSet::iterator CandidateIterator;

		CandidateSet candidateSet;
	};

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	void searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		integer kNearest,
		PASTEL_NO_DEDUCTION((std::vector<typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>))* nearestSet,
		PASTEL_NO_DEDUCTION(std::vector<Real>)* distanceSet)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);
		ENSURE1(kNearest >= 0, kNearest);
		ENSURE2(kNearest < kdTree.objects(), kNearest, kdTree.objects());
		ENSURE2(!distanceSet || distanceSet->size() == kNearest,
			distanceSet->size(), kNearest);
		ENSURE2(!nearestSet || nearestSet->size() == kNearest,
			nearestSet->size(), kNearest);

		SearchNearest<N, Real, ObjectPolicy, NormBijection> searchFunctor(
			kdTree, searchPoint, maxDistance, maxRelativeError,
			normBijection, kNearest, nearestSet, distanceSet);

		searchFunctor.work();
	}

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator> 
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		if (kdTree.empty())
		{
			return keyValue(infinity<Real>(), kdTree.end());
		}

		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;

		std::vector<Real> distanceSet(1);
		std::vector<ConstObjectIterator> nearestSet(1);

		searchNearest(kdTree, point, maxDistance, maxRelativeError,
			normBijection, 1, &nearestSet, &distanceSet);

		if (nearestSet.empty())
		{
			return keyValue(infinity<Real>(), kdTree.end());
		}

		return keyValue(distanceSet.front(), nearestSet.front());
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		return Pastel::searchNearest(
			kdTree, point, maxDistance, maxRelativeError,
			EuclideanNormBijection<N, Real>());
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		ENSURE1(maxDistance >= 0, maxDistance);

		return Pastel::searchNearest(
			kdTree, point, maxDistance, 0,
			EuclideanNormBijection<N, Real>());
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		searchNearest(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const Point<N, Real>& point)
	{
		return Pastel::searchNearest(
			kdTree, point, infinity<Real>(), 0,
			EuclideanNormBijection<N, Real>());
	}

}

#endif
