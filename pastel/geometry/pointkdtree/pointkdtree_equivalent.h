// Description: Equivalance of point kd-trees

#ifndef PASTELGEOMETRY_POINTKDTREE_EQUIVALENT_H
#define PASTELGEOMETRY_POINTKDTREE_EQUIVALENT_H

#include "pastel/geometry/pointkdtree/pointkdtree.h"

namespace Pastel
{

	//! Returns whether the kd-trees are equivalent.
	template <
		typename A_Settings, template <typename> class A_Customization,
		typename B_Settings, template <typename> class B_Customization> 
	bool equivalent(
		const PointKdTree<A_Settings, A_Customization>& aTree,
		const PointKdTree<B_Settings, B_Customization>& bTree);

}

#include "pastel/geometry/pointkdtree/pointkdtree_equivalent.hpp"

#endif
