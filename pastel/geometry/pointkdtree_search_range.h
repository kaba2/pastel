// Description: Orthogonal range search in a kd-tree
// Documentation: pointkdtree_search_range.txt

#ifndef PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_H
#define PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/alignedbox.h"

#include <vector>

namespace Pastel
{

	//! Finds all points contained in an aligned box using a kd-tree.
	/*!
	Preconditions:
	kdTree.n() == range.n()
	kdTree.n() <= 32
	bucketSize >= 1
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename Point_ConstIterator_Iterator>
	void searchRange(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		Point_ConstIterator_Iterator result,
		integer bucketSize = 8);

}

#include "pastel/geometry/pointkdtree_search_range.hpp"

#endif
