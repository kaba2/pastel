#ifndef PASTEL_COUNT_ALL_RANGE_POINTKDTREE_H
#define PASTEL_COUNT_ALL_RANGE_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/iterator_range.h"

namespace Pastel
{

	//! Counts the number of neighbors for all query points.
	template <typename Real, int N, typename PointPolicy,
		typename Point_ConstIterator_Iterator,
		typename Real_Iterator,
		typename Integer_OutputIterator>
	void countAllRange(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const RandomAccessIterator_Range<Point_ConstIterator_Iterator>& querySet,
		const RandomAccessIterator_Range<Real_Iterator>& maxDistanceSet,
		Integer_OutputIterator result,
		integer bucketSize = 8);

}

#include "pastel/geometry/count_all_range_pointkdtree.hpp"

#endif
