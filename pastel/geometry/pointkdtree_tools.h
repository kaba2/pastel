// Description: Basic algorithms for PointKdTree

#ifndef PASTELGEOMETRY_POINTKDTREE_TOOLS_H
#define PASTELGEOMETRY_POINTKDTREE_TOOLS_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/splitrules.h"
#include "pastel/geometry/pointkdtree_search_nearest.h"
#include "pastel/geometry/pointkdtree_count_nearest.h"
#include "pastel/geometry/pointkdtree_search_range.h"

namespace Pastel
{

	//! Compute the maximum depth of the kd-tree.

	template <typename Real, int N,
		typename PointPolicy>
	integer depth(const PointKdTree<Real, N, PointPolicy>& tree);

	//! Check the invariants of the kd-tree.
	/*!
	This function is used for debugging purposes
	to ensure that the kd-tree correctly maintains
	its invariants.
	*/

	template <typename Real, int N,
		typename PointPolicy>
	bool check(const PointKdTree<Real, N, PointPolicy>& tree);

	//! Check that the kd-trees are equivalent.

	template <int N_A, typename Real, typename PointPolicy_A, 
		int N_B, typename PointPolicy_B>
	bool equivalent(
		const PointKdTree<Real, N_A, PointPolicy_A>& aTree,
		const PointKdTree<Real, N_B, PointPolicy_B>& bTree);

}

#include "pastel/geometry/pointkdtree_tools.hpp"

#endif
