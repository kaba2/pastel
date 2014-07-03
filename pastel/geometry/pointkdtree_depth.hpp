#ifndef PASTELGEOMETRY_POINTKDTREE_DEPTH_HPP
#define PASTELGEOMETRY_POINTKDTREE_DEPTH_HPP

#include "pastel/geometry/pointkdtree_depth.h"

namespace Pastel
{

	namespace PointKdTree_Depth_
	{

		template <typename Real, int N, typename PointPolicy>
		integer depth(
			const PointKdTree<Real, N, PointPolicy>& tree,
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
		return PointKdTree_Depth_::depth(tree, tree.root(), 0);
	}

}

#endif
