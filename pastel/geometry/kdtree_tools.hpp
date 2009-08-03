#ifndef PASTEL_KDTREETOOLS_HPP
#define PASTEL_KDTREETOOLS_HPP

#include "pastel/geometry/kdtree_tools.h"
#include "pastel/geometry/intersect_line_alignedbox.h"
#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/geometry/box_area.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/math_functions.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/syscommon.h"
#include "pastel/sys/vector_tools.h"

#include <algorithm>
#include <vector>

namespace Pastel
{

	inline integer computeKdTreeMaxDepth(integer objects)
	{
		ENSURE_OP(objects, >=, 0);
		return (real)1.3 * (real)integerLog2(objects + 1) + (real)8.0;
	}

	namespace Detail
	{

		template <typename Real, int N, typename ObjectPolicy>
		integer depth(const KdTree<Real, N, ObjectPolicy>& tree,
			const typename KdTree<Real, N, ObjectPolicy>::Cursor& cursor,
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

	template <typename Real, int N, typename ObjectPolicy>
	integer depth(const KdTree<Real, N, ObjectPolicy>& tree)
	{
		return Detail::depth(tree, tree.root(), 0);
	}

	namespace Detail
	{

		template <typename Real, int N, typename ObjectPolicy>
		bool check(const KdTree<Real, N, ObjectPolicy>& tree,
			const typename KdTree<Real, N, ObjectPolicy>::Cursor& cursor,
			const AlignedBox<Real, N>& bound)
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

				AlignedBox<Real, N> positiveBound(bound);
				positiveBound.min()[cursor.splitAxis()] = cursor.splitPosition();

				if (!check(tree, cursor.positive(), positiveBound))
				{
					return false;
				}

				AlignedBox<Real, N> negativeBound(bound);
				negativeBound.max()[cursor.splitAxis()] = cursor.splitPosition();

				if (!check(tree, cursor.negative(), negativeBound))
				{
					return false;
				}
			}

			return true;
		}

	}

	template <typename Real, int N, typename ObjectPolicy>
	bool check(const KdTree<Real, N, ObjectPolicy>& tree)
	{
		return Detail::check(tree, tree.root(), tree.bound());
	}

	namespace Detail_EquivalentKdTree
	{

		template <typename CursorA, 
			typename CursorB>
			bool equivalentKdTree(
			const CursorA& aTree,
			const CursorB& bTree)
		{
			if (aTree.leaf() != bTree.leaf())
			{
				return false;
			}

			if (aTree.leaf())
			{
				if (aTree.objects() != bTree.objects())
				{
					return false;
				}
			}
			else
			{
				if (aTree.splitAxis() != bTree.splitAxis() ||
					aTree.splitPosition() != bTree.splitPosition())
				{
					return false;
				}

				if (!equivalentKdTree(aTree.negative(), bTree.negative()))
				{
					return false;
				}
				if (!equivalentKdTree(aTree.positive(), bTree.positive()))
				{
					return false;
				}
			}			

			return true;
		}

	}
	template <int N_A, typename Real, typename ObjectPolicy_A, 
		int N_B, typename ObjectPolicy_B>
	bool equivalentKdTree(const KdTree<Real, N_A, ObjectPolicy_A>& aTree,
	const KdTree<Real, N_B, ObjectPolicy_B>& bTree)
	{
		if (aTree.nodes() != bTree.nodes() ||
			aTree.objects() != bTree.objects() ||
			aTree.leaves() != bTree.leaves() ||
			aTree.dimension() != bTree.dimension())
		{
			return false;
		}
		/*
			!allEqual(aTree.bound().min(), bTree.bound().min()) ||
			!allEqual(aTree.bound().max(), bTree.bound().max()))
		*/

		return Detail_EquivalentKdTree::equivalentKdTree(
			aTree.root(), bTree.root());
	}
}

#endif
