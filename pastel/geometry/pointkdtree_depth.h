// Description: Maximum depth of a kd-tree

#ifndef PASTELGEOMETRY_POINTKDTREE_DEPTH_H
#define PASTELGEOMETRY_POINTKDTREE_DEPTH_H

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	//! Returns the maximum depth of the kd-tree.
	template <typename Settings, template <typename> class Customization>
	integer depth(const PointKdTree<Settings, Customization>& tree);

}

#include "pastel/geometry/pointkdtree_depth.hpp"

#endif
