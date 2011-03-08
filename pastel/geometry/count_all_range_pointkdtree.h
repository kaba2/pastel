// Description: Orthogonal range counting for many query cubes using kd-tree
// Documentation: search_range.txt

#ifndef PASTEL_COUNT_ALL_RANGE_POINTKDTREE_H
#define PASTEL_COUNT_ALL_RANGE_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/iterator_range.h"

namespace Pastel
{

	//! Counts the number of points contained in each query cube.
	/*!
	Preconditions:
	bucketSize > 0
	querySet.size() == maxDistanceSet.size()

	kdTree:
	The kd-tree to count the points in.

	querySet:
	A set of iterators, each of which dereferences to a point iterator
	in the 'kdTree'. 

	maxDistanceSet:
	The maximum norm distance in which the neighbors are counted
	for each query point. The query range of a query point 'i' is an 
	_open_ aligned box, centered on the query point, with a side length
	of 2 * maxDistanceSet[i]. 

	result:
	An output iterator which receives the range counts for each
	query point, in the same order as the 'querySet'.

	bucketSize:
	The number of points in a subtree of 'kdTree', under which to 
	perform brute-force searching. A number greater than 1 usually
	gives better performance.

	Note: The sets must be random-access so that the search can be 
	parallelized.
	*/
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
