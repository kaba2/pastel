#ifndef PASTEL_SEARCH_RANGE_ALGORITHM_POINTKDTREE_HPP
#define PASTEL_SEARCH_RANGE_ALGORITHM_POINTKDTREE_HPP

#include "pastel/geometry/search_range_algorithm_pointkdtree.h"
#include "pastel/geometry/overlaps_alignedbox_point.h"
#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"

#include "pastel/sys/countingiterator.h"

namespace Pastel
{

	template <typename Real, int N, typename PointPolicy, 
		typename Reporter_SearchRange>
	void searchRangeAlgorithm(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		const Reporter_SearchRange& reporter,
		integer bucketSize)
	{
		ENSURE_OP(range.dimension(), ==, kdTree.dimension());

		typedef typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator
			ConstObjectIterator;
		typedef typename PointKdTree<Real, N, PointPolicy>::Cursor
			Cursor;
		typedef typename PointKdTree<Real, N, PointPolicy>::Object
			Object;

		// Note: we assume the search region is open.

		if (kdTree.empty())
		{
			return;
		}

		const integer dimension = kdTree.dimension();

		ENSURE_OP(dimension, <=, 32);
		ENSURE_OP(bucketSize, >=, 1);

		const uint32 fullFlags = ((uint32)1 << dimension) - 1;

		const Vector<Real, N>& rangeMin = range.min();
		const Vector<Real, N>& rangeMax = range.max();

		const PointPolicy& pointPolicy = kdTree.pointPolicy();

		const AlignedBox<Real, N>& bound = kdTree.bound();

		uint32 rootFlags = 0;
		for (integer i = 0;i < dimension;++i)
		{
			if (bound.max()[i] <= rangeMin[i] ||
				bound.min()[i] >= rangeMax[i])
			{
				// The search range does not
				// intersect the kd-tree.
				return;
			}

			if (rangeMin[i] < bound.min()[i] && 
				rangeMax[i] > bound.max()[i])
			{
				rootFlags += (1 << i);
			}
		}

		std::vector<std::pair<Cursor, uint32> > nodeSet;
		nodeSet.push_back(std::make_pair(kdTree.root(), rootFlags));

		while(!nodeSet.empty())
		{
			Cursor cursor = nodeSet.back().first;
			uint32 flags = nodeSet.back().second;
			nodeSet.pop_back();

			bool foundSomething = true;
			while(!cursor.leaf() && 
				cursor.objects() > bucketSize)
			{
				const integer splitAxis = cursor.splitAxis();
				const Real splitPosition = cursor.splitPosition();
				const uint32 flag = (uint32)1 << splitAxis;

				if (flags == fullFlags)
				{
					// The node is being stabbed on all axes. 
					// Stop recursion.
					break;
				}

				const Cursor left = cursor.left();
				const Cursor right = cursor.right();

				const Real leftMin = left.min();
				const Real leftMax = left.max();
				const Real rightMin = right.min();
				const Real rightMax = right.max();

				bool visitRight = false;
				bool visitLeft = false;
				uint32 leftFlags = flags;
				uint32 rightFlags = flags;

				if (rangeMin[splitAxis] < leftMax &&
					rangeMax[splitAxis] > leftMin)
				{
					// The left child intersects the query box.
					visitLeft = true;

					if (rangeMin[splitAxis] < leftMin &&
						rangeMax[splitAxis] > leftMax)
					{
						// The left child is being stabbed.
						leftFlags |= flag;
					}
				}

				if (rangeMin[splitAxis] < rightMax &&
					rangeMax[splitAxis] > rightMin)
				{
					// The right child intersects the query box.
					visitRight = true;

					if (rangeMin[splitAxis] < rightMin &&
						rangeMax[splitAxis] > rightMax)
					{
						// The right child is being stabbed.
						rightFlags |= flag;
					}
				}

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
					if (visitLeft)
					{
						cursor = cursor.left();
						flags = leftFlags;
					}
					else
					{
						foundSomething = false;
						break;
					}
				}
			}

			if (!foundSomething)
			{
				continue;
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
					const typename PointPolicy::Object& object = iter->object();
					// Cull the point dimension by dimension.
					integer i = 0;
					while(i < dimension)
					{
						const Real position = 
							pointPolicy.point(object, i);
						if (position <= rangeMin[i] || 
							position >= rangeMax[i])
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
