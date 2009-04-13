#ifndef PASTELGEOMETRY_KDTREE_NEAREST_HPP
#define PASTELGEOMETRY_KDTREE_NEAREST_HPP

#include "pastel/geometry/kdtree_nearest.h"

#include "pastel/sys/smallset.h"

#include <vector>

namespace Pastel
{

	namespace Detail_FindNearest
	{

		template <int N, typename Real, typename ObjectPolicy>
		class StackEntry
		{
		public:
			StackEntry()
				: node_()
				, distance_(0)
			{
			}

			StackEntry(
				const typename KdTree<N, Real, ObjectPolicy>::Cursor& node,
				const Real& distance)
				: node_(node)
				, distance_(distance)
			{
			}

			typename KdTree<N, Real, ObjectPolicy>::Cursor node_;
			Real distance_;
		};

	}

	template <int N, typename Real, typename ObjectPolicy, typename NormFunctor>
	void findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormFunctor& normFunctor,
		integer kNearest,
		SmallSet<KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator> >& result)
	{
		ENSURE1(maxDistance >= 0, maxDistance);

		// Note that all norms are equivalent when considering
		// the distance from from a fixed point:
		//
		// |t_1 D| < |t_2 D|
		// <=>
		// |t_1| |D| < |t_2| |D|
		// <=>
		// |t_1| < |t_2|
		//
		// Thus the norm only has an effect on the result
		// when maxDistance is finite.

		result.clear();

		if (tree.empty() || kNearest == 0)
		{
			return;
		}

		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;

		typedef SmallSet<KeyVal> ResultContainer;
		typedef Detail_FindNearest::StackEntry<N, Real, ObjectPolicy> StackEntry;

		const bool kFinite = (kNearest > 0);
		Real cullDistance = maxDistance;

		std::vector<StackEntry> stack;
		stack.push_back(StackEntry(tree.root(), -1));

		while(!stack.empty())
		{
			StackEntry stackEntry = stack.back();
			stack.pop_back();

			if (stackEntry.distance_ > cullDistance)
			{
				// The node is beyond cull distance, skip it.
				continue;
			}

			// Find the leaf node that is closest
			// to the search searchPoint in this current branch.
			// Place all the alternative branching nodes into
			// a stack.

			Cursor cursor(stackEntry.node_);
			while(!cursor.leaf())
			{
				const Real planeDistance =
					searchPoint[cursor.splitAxis()] - cursor.splitPosition();

				if (searchPoint[cursor.splitAxis()] < cursor.splitPosition())
				{
					// The search point is closer to the left branch so follow that.
					// Place right branch into the stack.
					stack.push_back(StackEntry(cursor.positive(), -planeDistance));
					cursor = cursor.negative();
				}
				else
				{
					// The search point is closer to the right branch so follow that.
					// Place left branch into the stack.
					stack.push_back(StackEntry(cursor.negative(), planeDistance));
					cursor = cursor.positive();
				}
			}
			
			// Beginning from the node which contains the search
			// point, start expanding the neighborhood
			// to search for nearest neighbors.

			ConstObjectIterator iter = cursor.begin();
			ConstObjectIterator iterEnd = cursor.end();

			while(iter != iterEnd)
			{
				const Point<N, Real> objectPoint = tree.objectPolicy().bound(*iter).min();

				const Real currentDistance =
					normFunctor(objectPoint - searchPoint);

				// It is essential that this is <= rather
				// than <, because of the possibility
				// of multiple points at same location.
				if (currentDistance <= cullDistance)
				{
					result.insert(
						KeyVal(currentDistance, iter));

					if (kFinite && result.size() > kNearest)
					{
						result.pop_back();

						// Since we now know k neighboring points,
						// we can bound the search radius
						// by the distance to the currently farthest
						// neighbor.

						cullDistance = result.back().key();
					}
				}

				++iter;
			}
		}
	}

	template <int N, typename Real, typename ObjectPolicy, typename NormFunctor>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormFunctor& normFunctor)
	{
		ENSURE1(maxDistance >= 0, maxDistance);

		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;

		SmallSet<KeyVal> result;
		findNearest(tree, point, maxDistance, normFunctor, 1, result);

		if (result.empty())
		{
			return keyValue(infinity<Real>(), tree.end());
		}

		return keyValue(result.front().key(), result.front().value());
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		ENSURE1(maxDistance >= 0, maxDistance);

		return Pastel::findNearest(
			tree, point, maxDistance, norm<N, Real>);
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point)
	{
		return Pastel::findNearest(
			tree, point, infinity<Real>(), norm<N, Real>);
	}

}

#endif
