#ifndef PASTEL_COUNT_ALL_RANGE_POINTKDTREE_H
#define PASTEL_COUNT_ALL_RANGE_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/randomaccessrange.h"

namespace Pastel
{

	//! Counts the number of neighbors for all query points.
	template <typename Real, int N, typename PointPolicy,
		typename ConstObjectIterator_Iterator,
		typename Real_Iterator,
		typename Integer_OutputIterator>
	void countAllRange(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet,
		Integer_OutputIterator result,
		integer bucketSize = 8);

}

#include "pastel/geometry/count_all_range_pointkdtree.hpp"

#endif
