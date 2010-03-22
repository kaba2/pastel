#ifndef PASTEL_SEARCH_RANGE_ALGORITHM_POINTKDTREE_HPP
#define PASTEL_SEARCH_RANGE_ALGORITHM_POINTKDTREE_HPP

#include "pastel/geometry/search_range_algorithm_pointkdtree.h"
#include "pastel/geometry/overlaps_alignedbox_point.h"
#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"

#include "pastel/sys/countingiterator.h"

namespace Pastel
{

	template <typename Real, int N, typename ObjectPolicy, 
		typename Reporter_SearchRange>
	void searchRangeAlgorithm(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		const Reporter_SearchRange& reporter,
		integer bucketSize)
	{
		ENSURE_OP(range.dimension(), ==, kdTree.dimension());

		typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
			ConstObjectIterator;
		typedef typename PointKdTree<Real, N, ObjectPolicy>::Cursor
			Cursor;
		typedef typename PointKdTree<Real, N, ObjectPolicy>::Object
			Object;

		if (kdTree.empty() || !overlaps(range, kdTree.bound()))
		{
			return;
		}

		const integer dimension = kdTree.dimension();

		ENSURE_OP(dimension, <=, 32);
		ENSURE_OP(bucketSize, >=, 1);

		const uint32 fullFlags = ((uint32)1 << dimension) - 1;

		const Vector<Real, N>& rangeMin = range.min();
		const Vector<Real, N>& rangeMax = range.max();

		// Find out the stabbing flags for the root node.
		uint32 rootFlags = 0;
		for (integer i = 0;i < dimension;++i)
		{
			if (rangeMin[i] <= kdTree.bound().min()[i] &&
				rangeMax[i] >= kdTree.bound().max()[i])
			{
				rootFlags |= (uint32)1 << i;
			}
		}

		const ObjectPolicy& objectPolicy = kdTree.objectPolicy();

		std::vector<std::pair<Cursor, uint32> > nodeSet;
		nodeSet.push_back(std::make_pair(kdTree.root(), rootFlags));

		while(!nodeSet.empty())
		{
			Cursor cursor = nodeSet.back().first;
			uint32 flags = nodeSet.back().second;
			nodeSet.pop_back();

			// As an invariant, the node
			// intersects the query box.

			while(!cursor.leaf() && 
				cursor.objects() > bucketSize)
			{
				const integer splitAxis = cursor.splitAxis();
				const Real splitPosition = cursor.splitPosition();
				const uint32 flag = (uint32)1 << splitAxis;

				if ((flags & flag) == flag)
				{
					// This node is being stabbed
					// on the splitting axis.

					if (flags == fullFlags)
					{
						// Actually, this node is being
						// stabbed on all axes. Stop recursion.
						break;
					}
					
					// Visit both children.
					nodeSet.push_back(
						std::make_pair(cursor.left(), flags));
					cursor = cursor.right();
					continue;
				}

				bool visitRight = false;
				bool visitLeft = false;
				uint32 leftFlags = flags;
				uint32 rightFlags = flags;

				if (rangeMax[splitAxis] >= splitPosition)
				{
					// The right child intersects the query box.
					visitRight = true;

					if (rangeMin[splitAxis] <= cursor.min())
					{
						// The left child is being stabbed.
						leftFlags |= flag;
					}
				}
				if (rangeMin[splitAxis] <= splitPosition)
				{
					// The left child intersects the query box.
					visitLeft = true;

					if (rangeMax[splitAxis] >= cursor.max())
					{
						// The right child is being stabbed.
						rightFlags |= flag;
					}
				}

				ASSERT(visitLeft || visitRight);

				if (visitRight)
				{
					if (visitLeft)
					{
						nodeSet.push_back(
							std::make_pair(cursor.left(), leftFlags));
					}
					cursor = cursor.right();
					flags = rightFlags;
				}
				else
				{
					// Because of the invariant, at least one
					// child must intersect the search range.
					cursor = cursor.left();
					flags = leftFlags;
				}
			}
			
			// Collect the objects.

			if (flags == fullFlags)
			{
				// All objects in this subtree are
				// in the range.

				reporter.report(
					cursor.begin(), 
					cursor.end(),
					cursor.objects());
			}
			else
			{
				// Only accept objects in the range.

				ConstObjectIterator iter = cursor.begin();
				const ConstObjectIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					const typename ObjectPolicy::Object& object = iter->object();
					// Cull the point dimension by dimension.
					integer i = 0;
					while(i < dimension)
					{
						const Real position = 
							objectPolicy.point(object, i);
						if (position < rangeMin[i] || 
							position > rangeMax[i])
						{
							// The point is not in the range.
							break;
						}
						++i;
					}
					if (i == dimension)
					{
						reporter.report(iter);
					}

					++iter;
				}
			}
		}
	}

}

#endif
