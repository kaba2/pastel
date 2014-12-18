#ifndef PASTELGEOMETRY_POINTKDTREE_DEPTH_HPP
#define PASTELGEOMETRY_POINTKDTREE_DEPTH_HPP

#include "pastel/geometry/pointkdtree/pointkdtree_depth.h"

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

	template <typename Settings, template <typename> class Customization>
	integer depth(const PointKdTree<Settings, Customization>& tree)
	{
		return PointKdTree_Depth_::depth(tree, tree.root(), 0);
	}

}

#endif
