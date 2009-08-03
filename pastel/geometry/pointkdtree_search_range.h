#ifndef PASTEL_POINTKDTREE_SEARCH_RANGE_H
#define PASTEL_POINTKDTREE_SEARCH_RANGE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/alignedbox.h"

#include <vector>

namespace Pastel
{

	//! Finds all objects in a kd-tree within an orthogonal range.
	/*!
	Preconditions:
	kdTree.dimension() == range.dimension()

	More specifically, finds all objects whose
	bounding boxes intersect the orthogonal query range.
	*/

	template <int N, typename Real, typename ObjectPolicy>
	void searchRange(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		std::vector<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>& result);

}

#include "pastel/geometry/pointkdtree_search_range.hpp"

#endif
