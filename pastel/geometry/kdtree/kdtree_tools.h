// Description: Algorithms for KdTree
// Documentation: kdtree.txt

#ifndef PASTELGEOMETRY_KDTREE_TOOLS_H
#define PASTELGEOMETRY_KDTREE_TOOLS_H

#include "pastel/sys/math_functions.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/vector/vector.h"

#include "pastel/geometry/kdtree/kdtree.h"
#include "pastel/geometry/kdtree/kdtree_refine.h"
#include "pastel/geometry/kdtree/kdtree_raytraversal.h"
#include "pastel/geometry/intersect/intersect_line_alignedbox.h"
#include "pastel/geometry/bounding/bounding_alignedbox.h"
#include "pastel/geometry/area/box_area.h"
#include "pastel/geometry/distance/distance_point_point.h"

#include <algorithm>
#include <vector>

namespace Pastel
{

	//! Computes a good maximum depth value for a kd-tree.
	/*!
	Preconditions:
	objects >= 0
	*/
	inline integer computeKdTreeMaxDepth(integer objects)
	{
		ENSURE_OP(objects, >=, 0);
		return (dreal)1.3 * (dreal)integerLog2(objects + 1) + (dreal)8.0;
	}

	namespace KdTree_
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

	//! Computes the maximum depth of the kd-tree.
	template <typename Real, int N, typename ObjectPolicy>
	integer depth(const KdTree<Real, N, ObjectPolicy>& tree)
	{
		return KdTree_::depth(tree, tree.root(), 0);
	}

	namespace KdTree_
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

	//! Checks the invariants of the kd-tree.
	/*!
	This function is used for debugging purposes
	to ensure that the kd-tree correctly maintains
	its invariants.
	*/
	template <typename Real, int N, typename ObjectPolicy>
	bool check(const KdTree<Real, N, ObjectPolicy>& tree)
	{
		return KdTree_::check(tree, tree.root(), tree.bound());
	}

	namespace Equivalent_KdTree
	{

		template <typename CursorA, 
			typename CursorB>
			bool equivalent(
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

				if (!equivalent(aTree.negative(), bTree.negative()))
				{
					return false;
				}
				if (!equivalent(aTree.positive(), bTree.positive()))
				{
					return false;
				}
			}			

			return true;
		}

	}
	template <int N_A, typename Real, typename ObjectPolicy_A, 
		int N_B, typename ObjectPolicy_B>
	bool equivalent(const KdTree<Real, N_A, ObjectPolicy_A>& aTree,
	const KdTree<Real, N_B, ObjectPolicy_B>& bTree)
	{
		if (aTree.nodes() != bTree.nodes() ||
			aTree.objects() != bTree.objects() ||
			aTree.leaves() != bTree.leaves() ||
			aTree.n() != bTree.n())
		{
			return false;
		}
		/*
			!allEqual(aTree.bound().min(), bTree.bound().min()) ||
			!allEqual(aTree.bound().max(), bTree.bound().max()))
		*/

		return Equivalent_KdTree::equivalent(
			aTree.root(), bTree.root());
	}
}

#endif
