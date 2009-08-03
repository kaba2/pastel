#ifndef PASTEL_POINTKDTREE_SEARCH_RANGE_HPP
#define PASTEL_POINTKDTREE_SEARCH_RANGE_HPP

#include "pastel/geometry/search_range_pointkdtree.h"
#include "pastel/geometry/overlaps_alignedbox_point.h"
#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"

namespace Pastel
{

	template <int N, typename Real, typename ObjectPolicy>
	void searchRange(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		std::vector<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>& result)
	{
		ENSURE_OP(range.dimension(), ==, kdTree.dimension());

		typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
			ConstObjectIterator;
		typedef typename PointKdTree<Real, N, ObjectPolicy>::Cursor
			Cursor;
		typedef typename PointKdTree<Real, N, ObjectPolicy>::Object
			Object;

		std::vector<ConstObjectIterator> rangeSet;

		if (!overlaps(range, kdTree.bound()))
		{
			rangeSet.swap(result);
			return;
		}

		std::vector<Cursor> nodeSet;
		nodeSet.push_back(kdTree.root());

		while(!nodeSet.empty())
		{
			Cursor cursor = nodeSet.back();
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
				const Object& object = iter->object();
				if (overlaps(range, objectPolicy.point(object)))
				{
					rangeSet.push_back(iter);
				}
				++iter;
			}
		}

		rangeSet.swap(result);
	}

}

#endif
