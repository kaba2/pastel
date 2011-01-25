// Description: All-nearest-neighbors counting using PointKdTree

#ifndef PASTEL_COUNT_ALL_NEIGHBORS_POINTKDTREE_H
#define PASTEL_COUNT_ALL_NEIGHBORS_POINTKDTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/iterator_range.h"

#include "pastel/geometry/pointkdtree.h"

#include <vector>

namespace Pastel
{

	//! Counts the number of neighbors for all query points.
	/*!
	Preconditions:
	bucketSize > 0

	kdTree:
	A point kd-tree.

	querySet:
	A set of n point iterators to 'kdTree'. Denotes the set
	of query points for which the neighbors are counted.
	The iterator must be a random-access iterator.

	maxDistanceSet:
	A sequence of n reals denoting the maximum distance inside
	which points are considered neighbors. Note:
	the special iterator ConstantIterator is memory-efficient 
	if the maximum distance is the same for all query points.
	See 'pastel/sys/iterators.txt'. The iterator must be a
	random-access iterator.

	normBijection:
	Defines the distance that is used. 
	See 'pastel/math/normbijection.txt'.

	result (output):
	A sequence of n integers, the i:th member denoting the number 
	of neighbors 'querySet[i]' has. The iterator 
	must be a random-access iterator and writable.
	*/
	template <typename Real, int N, typename PointPolicy,
		typename Point_ConstIterator_Iterator,
		typename Real_Iterator,
		typename Integer_OutputIterator,
		typename NormBijection>
	void countAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const RandomAccessIterator_Range<Point_ConstIterator_Iterator>& querySet,
		const RandomAccessIterator_Range<Real_Iterator>& maxDistanceSet,
		Integer_OutputIterator result,
		integer bucketSize,
		const NormBijection& normBijection);

	//! Counts the number of neighbors for all query points.
	/*!
	This is a convenience function that calls:
	countAllNeighbors(
		kdTree, querySet,
		maxDistanceSet, result,
		bucketSize,
		Euclidean_NormBijection<Real>());
	*/
	template <typename Real, int N, typename PointPolicy,
		typename Point_ConstIterator_Iterator,
		typename Real_Iterator,
		typename Integer_OutputIterator>
	void countAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const RandomAccessIterator_Range<Point_ConstIterator_Iterator>& querySet,
		const RandomAccessIterator_Range<Real_Iterator>& maxDistanceSet,
		Integer_OutputIterator result,
		integer bucketSize = 8);

}

#include "pastel/geometry/count_all_neighbors_pointkdtree.hpp"

#endif
