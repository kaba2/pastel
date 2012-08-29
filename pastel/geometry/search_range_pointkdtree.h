// Description: Orthogonal range searching using a kd-tree
// Documentation: range_searching.txt

#ifndef PASTELGEOMETRY_SEARCH_RANGE_POINTKDTREE_H
#define PASTELGEOMETRY_SEARCH_RANGE_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/alignedbox.h"

#include <vector>

namespace Pastel
{

	//! Finds all points contained in an aligned box using a kd-tree.
	/*!
	Preconditions:
	kdTree.dimension() == range.dimension()
	kdTree.dimension() <= 32
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

#include "pastel/geometry/search_range_pointkdtree.hpp"

#endif
