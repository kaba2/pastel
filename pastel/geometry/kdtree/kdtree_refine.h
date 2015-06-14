// Description: Split rules for KdTree
// Documentation: kdtree.txt

#ifndef PASTELGEOMETRY_KDTREE_REFINE_H
#define PASTELGEOMETRY_KDTREE_REFINE_H

#include "pastel/geometry/kdtree/kdtree.h"

namespace Pastel
{

	//! Refines a kd-tree using the midpoint rule.
	/*!
	Preconditions:
	maxDepth >= 0
	maxObjects >= 1

	The midpoint rule splits a node
	from the middle along the axis on which the node
	has greatest extent.

	This refinement is naive and should not
	be used for anything. However, it is provided for 
	checking and performance comparison purposes.
	*/

	class Fair_SplitRule_KdTree;

	//! Refines a kd-tree using the sliding midpoint rule.
	/*!
	Preconditions:
	maxDepth >= 0
	maxObjects >= 1

	The sliding midpoint rule first tries to split a node
	from the middle along the axis on which the node
	has greatest extent. If all points are on the
	same side of the split plane, the split plane
	is slid to compactly bound those points, while still
	keeping them all on the same side.

	The sliding midpoint rule is ideal for searching
	nearest neighbors for points.
	*/

	class SlidingMidpoint_SplitRule_KdTree;

	//! Refines a kd-tree using the surface area heuristic.
	/*!
	Preconditions:
	maxDepth >= 0
	maxObjects >= 1

	The surface area heuristic tries to minimize
	the cost of traversing the kd-tree with a ray.
	*/
	template <
		typename Real,
		integer N,
		typename ObjectPolicy>
		void refineSurfaceAreaHeuristic(
		integer maxDepth,
		integer maxObjects,
		KdTree<Real, N, ObjectPolicy>& tree);

}

#include "pastel/geometry/kdtree/kdtree_refine.hpp"

#endif

