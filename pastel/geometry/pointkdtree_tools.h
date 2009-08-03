#ifndef PASTEL_KDTREE_TOOLS_H
#define PASTEL_KDTREE_TOOLS_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/pointkdtree_refine.h"
#include "pastel/geometry/pointkdtree_search_nearest.h"
#include "pastel/geometry/pointkdtree_count_nearest.h"
#include "pastel/geometry/pointkdtree_search_range.h"

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
	integer depth(const PointKdTree<Real, N, ObjectPolicy>& tree);

	//! Checks the invariants of the kd-tree.
	/*!
	This function is used for debugging purposes
	to ensure that the kd-tree correctly maintains
	its invariants.
	*/

	template <typename Real, int N,
		typename ObjectPolicy>
	bool check(const PointKdTree<Real, N, ObjectPolicy>& tree);

	template <int N_A, typename Real, typename ObjectPolicy_A, 
		int N_B, typename ObjectPolicy_B>
	bool equivalentKdTree(const PointKdTree<Real, N_A, ObjectPolicy_A>& aTree,
	const PointKdTree<Real, N_B, ObjectPolicy_B>& bTree);

}

#include "pastel/geometry/pointkdtree_tools.hpp"

#endif
