// Description: Point kd-tree invariants

#ifndef PASTELGEOMETRY_POINTKDTREE_INVARIANTS_H
#define PASTELGEOMETRY_POINTKDTREE_INVARIANTS_H

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	//! Returns whether the invariants hold for the point kd-tree.
	/*!
	This function is useful only for debugging;
	a correct implementation always returns true.
	*/
	template <typename Real, int N, typename PointPolicy>
	bool testInvariants(const PointKdTree<Real, N, PointPolicy>& tree);

}

#include "pastel/geometry/pointkdtree_invariants.hpp"

#endif
