#ifndef PASTEL_POINTKDTREE_REFINE_H
#define PASTEL_POINTKDTREE_REFINE_H

#include "pastel/geometry/pointkdtree.h"

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

	class Midpoint_SplitRule_PointKdTree;

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

	class SlidingMidpoint_SplitRule_PointKdTree;

	//! Refines a kd-tree using a modified sliding midpoint rule.
	/*!
	Preconditions:
	maxDepth >= 0
	maxObjects >= 1

	The modified sliding midpoint rule works exactly like
	the unmodified sliding midpoint rule, with the exception
	that when there are points to both sides of the plane,
	the plane is slid to touch that point set
	which has more points. This tends to give larger cell
	of empty space and thus better performance because the 
	process is biased towards cutting the empty space away from 
	the larger point set. This also guarantees that at least 
	one point lies on each splitting plane.

	This rule is ideal for searching nearest neighbors for points.
	*/
	class SlidingMidpoint2_SplitRule_PointKdTree;

	//! Refines a kd-tree using the surface area heuristic.
	/*!
	Preconditions:
	maxDepth >= 0
	maxObjects >= 1

	The surface area heuristic tries to minimize
	the cost of traversing the kd-tree with a ray.
	*/
	template <
		int N,
		typename Real,
		typename ObjectPolicy>
		void refineSurfaceAreaHeuristic(
		integer maxDepth,
		integer maxObjects,
		PointKdTree<Real, N, ObjectPolicy>& tree);

}

#include "pastel/geometry/pointkdtree_refine.hpp"

#endif

