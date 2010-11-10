// Description: Generic orthogonal range searching using PointKdTree
// Documentation: search_range.txt

#ifndef PASTEL_SEARCH_RANGE_ALGORITHM_POINTKDTREE_H
#define PASTEL_SEARCH_RANGE_ALGORITHM_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/alignedbox.h"

#include <vector>

namespace Pastel
{

	//! Finds all points within an aligned box from a PointKdTree.
	/*!
	Preconditions:
	kdTree.dimension() == range.dimension()
	kdTree.dimension() <= 32
	bucketSize >= 1
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
