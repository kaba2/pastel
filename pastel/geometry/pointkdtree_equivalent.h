// Description: Equivalance of point kd-trees

#ifndef PASTELGEOMETRY_POINTKDTREE_EQUIVALENT_H
#define PASTELGEOMETRY_POINTKDTREE_EQUIVALENT_H

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	//! Returns whether the kd-trees are equivalent.
	template <
		int N_A, typename Real, typename PointPolicy_A, 
		int N_B, typename PointPolicy_B>
	bool equivalent(
		const PointKdTree<Real, N_A, PointPolicy_A>& aTree,
		const PointKdTree<Real, N_B, PointPolicy_B>& bTree);

}

#include "pastel/geometry/pointkdtree_equivalent.hpp"

#endif
