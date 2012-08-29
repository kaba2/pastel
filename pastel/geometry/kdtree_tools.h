// Description: Algorithms for KdTree
// Documentation: kdtree.txt

#ifndef PASTELGEOMETRY_KDTREE_TOOLS_H
#define PASTELGEOMETRY_KDTREE_TOOLS_H

#include "pastel/geometry/kdtree.h"
#include "pastel/geometry/kdtree_refine.h"
#include "pastel/geometry/kdtree_raytraversal.h"

namespace Pastel
{

	//! Computes a good maximum depth value for a kd-tree.
	/*!
	Preconditions:
	objects >= 0
	*/

	integer computeKdTreeMaxDepth(integer objects);

	//! Computes the maximum depth of the kd-tree.

	template <typename Real, int N,
		typename ObjectPolicy>
	integer depth(const KdTree<Real, N, ObjectPolicy>& tree);

	//! Checks the invariants of the kd-tree.
	/*!
	This function is used for debugging purposes
	to ensure that the kd-tree correctly maintains
	its invariants.
	*/

	template <typename Real, int N,
		typename ObjectPolicy>
	bool check(const KdTree<Real, N, ObjectPolicy>& tree);

	template <int N_A, typename Real, typename ObjectPolicy_A, 
		int N_B, typename ObjectPolicy_B>
	bool equivalent(const KdTree<Real, N_A, ObjectPolicy_A>& aTree,
	const KdTree<Real, N_B, ObjectPolicy_B>& bTree);

}

#include "pastel/geometry/kdtree_tools.hpp"

#endif
