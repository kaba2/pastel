// Description: Generic orthogonal range search in a kd-tree
// Documentation: pointkdtree_search_range.txt

#ifndef PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_ALGORITHM_H
#define PASTELGEOMETRY_POINTKDTREE_SEARCH_RANGE_ALGORITHM_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/alignedbox.h"

#include <vector>

namespace Pastel
{

	//! Reports all points contained in an aligned box using a kd-tree.
	/*!
	Preconditions:
	kdTree.n() == range.n()
	kdTree.n() <= 64
	bucketSize > 0
	*/
	template <
		typename Settings, template <typename> class Customization,
		typename Output_SearchRange,
		typename Locator = typename Settings::Locator,
		typename Real = typename Locator::Real,
		integer N = Locator::N>
	void searchRangeAlgorithm(
		const PointKdTree<Settings, Customization>& kdTree,
		const PASTEL_NO_DEDUCTION((AlignedBox<Real, N>))& range,
		const Output_SearchRange& reporter,
		integer bucketSize = 8);

}

#include "pastel/geometry/pointkdtree_search_range_algorithm.hpp"

#endif
