// Description: Maximum depth of a kd-tree

#ifndef PASTELGEOMETRY_POINTKDTREE_DEPTH_H
#define PASTELGEOMETRY_POINTKDTREE_DEPTH_H

#include "pastel/geometry/pointkdtree/pointkdtree.h"

namespace Pastel
{

	namespace PointKdTree_Depth_
	{

		template <typename Settings, template <typename> class Customization>
		integer depth(
			const PointKdTree<Settings, Customization>& tree,
			const typename PointKdTree<Settings, Customization>::Cursor& cursor,
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

	//! Returns the maximum depth of the kd-tree.
	template <typename Settings, template <typename> class Customization>
	integer depth(const PointKdTree<Settings, Customization>& tree)
	{
		return PointKdTree_Depth_::depth(tree, tree.root(), 0);
	}

}

#endif
