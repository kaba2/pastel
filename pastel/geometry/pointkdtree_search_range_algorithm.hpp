#ifndef PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_ALGORITHM_HPP
#define PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_ALGORITHM_HPP

#include "pastel/geometry/pointkdtree_search_range_algorithm.h"
#include "pastel/geometry/overlaps_alignedbox_point.h"
#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"

#include "pastel/sys/counting_iterator.h"

namespace Pastel
{

	template <
		typename Settings, template <typename> class Customization,
		typename Output_SearchRange,
		typename Locator,
		typename Real,
		integer N>
	void searchRangeAlgorithm(
		const PointKdTree<Settings, Customization>& kdTree,
		const PASTEL_NO_DEDUCTION((AlignedBox<Real, N>))& range,
		const Output_SearchRange& reporter,
		integer bucketSize)
	{
		ENSURE_OP(range.n(), ==, kdTree.n());
		ENSURE_OP(kdTree.n(), <=, 64);
		ENSURE_OP(bucketSize, >, 0);

		using Fwd = PointKdTree<Settings, Customization>;
		PASTEL_FWD(Point_ConstIterator);
		PASTEL_FWD(Cursor);
		PASTEL_FWD(Point);

		// Note: we assume the search region is open.

		if (kdTree.empty() ||
			range.empty())
		{
			// Nothing to do.
			return;
		}

		using Flags = uint64;

		integer n = kdTree.n();
		Flags fullFlags = ((Flags)1 << n) - 1;


		const Vector<Real, N>& rangeMin = range.min();
		const Vector<Real, N>& rangeMax = range.max();

		const Locator& locator = kdTree.locator();

		const AlignedBox<Real, N>& bound = kdTree.bound();

		Flags rootFlags = 0;
		for (integer i = 0;i < n;++i)
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
				rootFlags += ((Flags)1 << i);
			}
		}

		std::vector<std::pair<Cursor, Flags> > nodeSet;
		nodeSet.push_back(std::make_pair(kdTree.root(), rootFlags));

		while(!nodeSet.empty())
		{
			Cursor cursor = nodeSet.back().first;
			Flags flags = nodeSet.back().second;
			nodeSet.pop_back();

			bool foundSomething = true;
			while(!cursor.leaf() && 
				cursor.points() > bucketSize)
			{
				integer splitAxis = cursor.splitAxis();
				Flags flag = (Flags)1 << splitAxis;

				if (flags == fullFlags)
				{
					// The node is being stabbed on all axes. 
					// Stop recursion.
					break;
				}

				Cursor left = cursor.left();
				Cursor right = cursor.right();

				Real leftMin = left.min();
				Real leftMax = left.max();
				Real rightMin = right.min();
				Real rightMax = right.max();

				bool visitRight = false;
				bool visitLeft = false;
				Flags leftFlags = flags;
				Flags rightFlags = flags;


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
			
			// Collect the points.

			if (flags == fullFlags)
			{
				// All points in this subtree are
				// in the range.

				reporter.report(
					cursor.begin(), 
					cursor.end(),
					cursor.points());
			}
			else
			{
				// Only accept points in the range.

				Point_ConstIterator iter = cursor.begin();
				Point_ConstIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{

					const auto& point = iter->point();
					// Cull the point dimension by n.
					integer i = 0;
					while(i < n)
					{
						Real position = 
							locator(point, i);
						if (position <= rangeMin[i] || 
							position >= rangeMax[i])
						{
							// The point is not in the range.
							break;
						}
						++i;
					}
					if (i == n)
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
