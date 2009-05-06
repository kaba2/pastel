#ifndef PASTELGEOMETRY_KDTREE_SEARCH_RANGE_HPP
#define PASTELGEOMETRY_KDTREE_SEARCH_RANGE_HPP

#include "pastel/geometry/kdtree_search_range.h"
#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"

namespace Pastel
{

	template <int N, typename Real, typename ObjectPolicy>
	void searchRange(
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const AlignedBox<N, Real>& range,
		std::vector<typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>& result)
	{
		ENSURE(range.dimension() == kdTree.dimension, 
			range.dimension(), kdTree.dimension());

		typedef typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator
			ConstObjectIterator;
		typedef typename KdTree<N, Real, ObjectPolicy>::Cursor
			Cursor;
		typedef typename KdTree<N, Real, ObjectPolicy>::Object
			Object;

		std::vector<ConstObjectIterator> rangeSet;
		std::vector<Cursor> nodeSet;

		while(!nodeSet.empty())
		{
			const Cursor cursor = nodeSet.back();
			nodeSet.pop_back();
			
			while(!cursor.leaf())
			{
				const integer splitAxis = cursor.splitAxis();
				const Real splitPosition = cursor.splitPosition();

				// As an invariant, the node intersects
				// the query box. Thus either one
				// or both children intersect
				// the query box.
				
				if (range.max()[splitAxis] >= splitPosition)
				{
					// The positive child intersects the query box.
					if (range.min()[splitAxis] < splitPosition)
					{
						// Both children intersect the query box.
						nodeSet.push_back(cursor.positive());
						cursor = cursor.negative();
					}
					else
					{
						// Only the positive child intersects
						// the query box. No need for stack.
						cursor = cursor.positive();
					}
				}
				else
				{
					// The positive child does not intersect the query box,
					// so the negative child must intersect the query box
					// by the invariant. No need for stack.
					cursor = cursor.negative();
				}
			}
			
			// We are now in a leaf node.
			// Collect the objects.

			const ObjectPolicy& objectPolicy = kdTree.objectPolicy();

			ConstObjectIterator iter = cursor.begin();
			const ConstObjectIterator iterEnd = cursor.end();
			while(iter != iterEnd)
			{
				const Object& object = *iter;
				if (overlaps(objectPolicy.bound(object), range))
				{
					result.push_back(object);
				}
				++iter;
			}
		}
	}

}

#endif
