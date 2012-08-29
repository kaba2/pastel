// Description: Generic orthogonal range searching using a kd-tree
// Documentation: range_searching.txt

#ifndef PASTELGEOMETRY_SEARCH_RANGE_ALGORITHM_POINTKDTREE_H
#define PASTELGEOMETRY_SEARCH_RANGE_ALGORITHM_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/alignedbox.h"

#include <vector>

namespace Pastel
{

	//! Reports all points contained in an aligned box using a kd-tree.
	/*!
	Preconditions:
	kdTree.dimension() == range.dimension()
	kdTree.dimension() <= 64
	bucketSize > 0
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename Reporter_SearchRange>
	void searchRangeAlgorithm(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		const Reporter_SearchRange& reporter,
		integer bucketSize = 8);

}

#include "pastel/geometry/search_range_algorithm_pointkdtree.hpp"

#endif
