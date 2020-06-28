// Description: Orthogonal range count in a point kd-tree
// Documentation: pointkdtree_search_range.txt

#ifndef PASTELGEOMETRY_POINTKDTREE_COUNT_RANGE_H
#define PASTELGEOMETRY_POINTKDTREE_COUNT_RANGE_H

#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/shape/alignedbox.h"

namespace Pastel
{

	//! Counts the number of points contained in an aligned box.
	/*!
	Preconditions:
	bucketSize > 0

	kdTree:
	The kd-tree to count the points in.

	range:
	An open aligned box for which to count the number
	of contained points.

	bucketSize:
	The number of points in a subtree of 'kdTree', under which to 
	perform brute-force searching. A number greater than 1 usually
	gives better performance.

	Returns:
	The number of points contained in the 'range'.
	*/
	template <
		typename Settings, template <typename> class Customization,
		typename Locator = typename Settings::Locator,
		typename Real = typename Locator::Real,
		int N = Locator::N>
	integer countRange(
		const PointKdTree<Settings, Customization>& kdTree,
		const NoDeduction<AlignedBox<Real, N>>& range,
		integer bucketSize = 8);

}

#include "pastel/geometry/pointkdtree/pointkdtree_count_range.hpp"

#endif
