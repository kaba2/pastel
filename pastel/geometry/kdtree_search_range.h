#ifndef PASTEL_KDTREE_SEARCH_RANGE_H
#define PASTEL_KDTREE_SEARCH_RANGE_H

#include "pastel/geometry/kdtree.h"
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
		const KdTree<N, Real, ObjectPolicy>& kdTree,
		const AlignedBox<N, Real>& range,
		std::vector<typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>& result);

}

#include "pastel/geometry/kdtree_search_range.hpp"

#endif
