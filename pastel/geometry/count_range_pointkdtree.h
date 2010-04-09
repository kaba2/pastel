// Description: Orthogonal range counting using PointKdTree
// Documentation: search_range.txt

#ifndef PASTEL_COUNT_RANGE_POINTKDTREE_H
#define PASTEL_COUNT_RANGE_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <typename Real, int N, typename PointPolicy>
	integer countRange(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		integer bucketSize = 8);

}

#include "pastel/geometry/count_range_pointkdtree.hpp"

#endif
