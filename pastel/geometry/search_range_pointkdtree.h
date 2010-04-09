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
	kdTree.dimension() <= 32
	bucketSize >= 1
	*/

	template <typename Real, int N, typename PointPolicy, 
		typename ObjectIterator_OutputIterator>
	void searchRange(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		ObjectIterator_OutputIterator result,
		integer bucketSize = 8);

	template <typename Real, int N, typename PointPolicy, 
		typename CandidateFunctor>
	void searchRangeGeneric(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const AlignedBox<Real, N>& range,
		const CandidateFunctor& candidateFunctor,
		integer bucketSize = 8);

}

#include "pastel/geometry/search_range_pointkdtree.hpp"

#endif
