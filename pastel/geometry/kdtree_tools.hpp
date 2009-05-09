#ifndef PASTELGEOMETRY_KDTREETOOLS_HPP
#define PASTELGEOMETRY_KDTREETOOLS_HPP

#include "pastel/geometry/kdtree_tools.h"
#include "pastel/geometry/intersect_line_alignedbox.h"
#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/geometry/boxarea.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/math/linear.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/syscommon.h"
#include "pastel/sys/vector_tools.h"

#include <algorithm>
#include <vector>

namespace Pastel
{

	inline integer computeKdTreeMaxDepth(integer objects)
	{
		ENSURE1(objects >= 0, objects);
		return (real)1.3 * (real)integerLog2(objects + 1) + (real)8.0;
	}

	namespace Detail
	{

		template <int N, typename Real, typename ObjectPolicy>
		integer depth(const KdTree<N, Real, ObjectPolicy>& tree,
			const typename KdTree<N, Real, ObjectPolicy>::Cursor& cursor,
			integer currentDepth)
		{
			if (cursor.leaf())
			{
				return currentDepth;
			}

			return std::max(
				depth(tree, cursor.positive(), currentDepth + 1),
				depth(tree, cursor.negative(), currentDepth + 1));
		}

	}

	template <int N, typename Real, typename ObjectPolicy>
	integer depth(const KdTree<N, Real, ObjectPolicy>& tree)
	{
		return Detail::depth(tree, tree.root(), 0);
	}

	namespace Detail
	{

		template <int N, typename Real, typename ObjectPolicy>
		bool check(const KdTree<N, Real, ObjectPolicy>& tree,
			const typename KdTree<N, Real, ObjectPolicy>::Cursor& cursor,
			const AlignedBox<N, Real>& bound)
		{
			if (cursor.leaf())
			{
				if (cursor.objects() == 0)
				{
					if (REPORT(cursor.begin() != tree.end() ||
						cursor.end() != tree.end()))
					{
						return false;
					}
				}
				else
				{
					if (REPORT(cursor.begin() == cursor.end()))
					{
						return false;
					}
					if (REPORT(std::distance(cursor.begin(), cursor.end()) != cursor.objects()))
					{
						return false;
					}
				}
			}
			else
			{
				if (REPORT(cursor.splitPosition() < bound.min()[cursor.splitAxis()]))
				{
					return false;
				}

				if (REPORT(cursor.splitPosition() > bound.max()[cursor.splitAxis()]))
				{
					return false;
				}

				if (REPORT(cursor.min() != bound.min()[cursor.splitAxis()]))
				{
					return false;
				}

				if (REPORT(cursor.max() != bound.max()[cursor.splitAxis()]))
				{
					return false;
				}

				AlignedBox<N, Real> positiveBound(bound);
				positiveBound.min()[cursor.splitAxis()] = cursor.splitPosition();

				if (!check(tree, cursor.positive(), positiveBound))
				{
					return false;
				}

				AlignedBox<N, Real> negativeBound(bound);
				negativeBound.max()[cursor.splitAxis()] = cursor.splitPosition();

				if (!check(tree, cursor.negative(), negativeBound))
				{
					return false;
				}
			}

			return true;
		}

	}

	template <int N, typename Real, typename ObjectPolicy>
	bool check(const KdTree<N, Real, ObjectPolicy>& tree)
	{
		return Detail::check(tree, tree.root(), tree.bound());
	}

}

#endif
