#ifndef PASTEL_POINTKDTREE_TOOLS_HPP
#define PASTEL_POINTKDTREE_TOOLS_HPP

#include "pastel/geometry/pointkdtree_tools.h"
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

	namespace Detail
	{

		template <typename Real, int N, typename PointPolicy>
		integer depth(const PointKdTree<Real, N, PointPolicy>& tree,
			const typename PointKdTree<Real, N, PointPolicy>::Cursor& cursor,
			integer currentDepth)
		{
			if (cursor.leaf())
			{
				return currentDepth;
			}

			return std::max(
				depth(tree, cursor.right(), currentDepth + 1),
				depth(tree, cursor.left(), currentDepth + 1));
		}

	}

	template <typename Real, int N, typename PointPolicy>
	integer depth(const PointKdTree<Real, N, PointPolicy>& tree)
	{
		return Detail::depth(tree, tree.root(), 0);
	}

	namespace Detail
	{

		template <typename Real, int N, typename PointPolicy>
		bool check(const PointKdTree<Real, N, PointPolicy>& tree,
			const typename PointKdTree<Real, N, PointPolicy>::Cursor& cursor,
			const AlignedBox<Real, N>& bound)
		{
			typedef typename PointKdTree<Real, N, PointPolicy>::Cursor Cursor;

			typedef typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator
				ConstObjectIterator;

			if (REPORT(cursor.empty() && cursor.parent().exists()))
			{
				return false;
			}

			if (cursor.leaf())
			{
				if (REPORT(std::distance(cursor.begin(), cursor.end()) != cursor.objects()))
				{
					return false;
				}

				if (REPORT(cursor.empty() && (cursor.begin() != tree.end() ||
					cursor.end() != tree.end())))
				{
					return false;
				}

				ConstObjectIterator iter = cursor.begin();
				const ConstObjectIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					Cursor leaf = iter->leaf();
					if (REPORT(leaf != cursor))
					{
						return false;
					}
					++iter;
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

				/*
				if (REPORT(cursor.min() >= bound.min()[cursor.splitAxis()]))
				{
					return false;
				}

				if (REPORT(cursor.max() <= bound.max()[cursor.splitAxis()]))
				{
					return false;
				}
				*/

				if (REPORT(cursor.left().objects() + cursor.right().objects() !=
					cursor.objects()))
				{
					return false;
				}

				if (REPORT(cursor.left().parent() != cursor))
				{
					return false;
				}

				if (REPORT(cursor.right().parent() != cursor))
				{
					return false;
				}

				if (cursor.empty())
				{
					if (REPORT(cursor.begin() != tree.end() || cursor.end() != tree.end()))
					{
						return false;
					}
				}
				else
				{
					ConstObjectIterator begin = 
						!cursor.left().empty() ? cursor.left().begin() : cursor.right().begin();
					ConstObjectIterator end =
						!cursor.right().empty() ? cursor.right().end() : cursor.left().end();
					if (REPORT(cursor.begin() != begin || cursor.end() != end))
					{
						return false;
					}
				}

				AlignedBox<Real, N> rightBound(bound);
				rightBound.min()[cursor.splitAxis()] = cursor.splitPosition();

				if (!check(tree, cursor.right(), rightBound))
				{
					return false;
				}

				AlignedBox<Real, N> leftBound(bound);
				leftBound.max()[cursor.splitAxis()] = cursor.splitPosition();

				if (!check(tree, cursor.left(), leftBound))
				{
					return false;
				}
			}

			return true;
		}

	}

	template <typename Real, int N, typename PointPolicy>
	bool check(const PointKdTree<Real, N, PointPolicy>& tree)
	{
		const AlignedBox<Real, N> bound(
			Vector<Real, N>(ofDimension(tree.dimension()), -infinity<Real>()),
			Vector<Real, N>(ofDimension(tree.dimension()), infinity<Real>()));;
		
		return Detail::check(tree, tree.root(), bound);
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
				if (aTree.min() != bTree.min() ||
					aTree.max() != bTree.max() ||
					aTree.splitAxis() != bTree.splitAxis() ||
					aTree.splitPosition() != bTree.splitPosition())
				{
					return false;
				}

				if (!equivalentKdTree(aTree.left(), bTree.left()))
				{
					return false;
				}
				if (!equivalentKdTree(aTree.right(), bTree.right()))
				{
					return false;
				}
			}			

			return true;
		}

	}
	template <int N_A, typename Real, typename PointPolicy_A, 
		int N_B, typename PointPolicy_B>
	bool equivalentKdTree(const PointKdTree<Real, N_A, PointPolicy_A>& aTree,
	const PointKdTree<Real, N_B, PointPolicy_B>& bTree)
	{
		if (aTree.nodes() != bTree.nodes() ||
			aTree.objects() != bTree.objects() ||
			aTree.leaves() != bTree.leaves() ||
			aTree.dimension() != bTree.dimension())
		{
			return false;
		}

		return Detail_EquivalentKdTree::equivalentKdTree(
			aTree.root(), bTree.root());
	}
}

#endif
