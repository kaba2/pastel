// Description: Orthogonal range searching using PointKdTree
// Documentation: search_range.txt

#ifndef PASTEL_SEARCH_RANGE_POINTKDTREE_H
#define PASTEL_SEARCH_RANGE_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/alignedbox.h"

#include <vector>

namespace Pastel
{

	//! Finds all objects within an aligned box from a PointKdTree.
	/*!
	Preconditions:
	kdTree.dimension() == range.dimension()
	*/

	template <typename Real, int N, typename ObjectPolicy>
	void searchRange(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		std::vector<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>& result);

}

#include "pastel/geometry/search_range_pointkdtree.hpp"

#endif
