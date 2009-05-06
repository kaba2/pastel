#ifndef PASTELGEOMETRY_KDTREETOOLS_H
#define PASTELGEOMETRY_KDTREETOOLS_H

#include "pastel/geometry/kdtree.h"
#include "pastel/geometry/kdtree_refine.h"
#include "pastel/geometry/kdtree_raytraversal.h"
#include "pastel/geometry/kdtree_search_nearest.h"
#include "pastel/geometry/kdtree_search_range.h"

namespace Pastel
{

	//! Computes a good maximum depth value for a kd-tree.
	/*!
	Preconditions:
	objects >= 0
	*/

	integer computeKdTreeMaxDepth(integer objects);

	//! Computes the maximum depth of the kd-tree.

	template <int N, typename Real,
		typename ObjectPolicy>
	integer depth(const KdTree<N, Real, ObjectPolicy>& tree);

	//! Checks the invariants of the kd-tree.
	/*!
	This function is used for debugging purposes
	to ensure that the kd-tree correctly maintains
	its invariants.
	*/

	template <int N, typename Real,
		typename ObjectPolicy>
	bool check(const KdTree<N, Real, ObjectPolicy>& tree);

}

#include "pastel/geometry/kdtree_tools.hpp"

#endif
