// Description: All-nearest-neighbors counting using PointKdTree
// Documentation: nearest_neighbors.txt

#ifndef PASTEL_COUNT_ALL_NEIGHBORS_POINTKDTREE_H
#define PASTEL_COUNT_ALL_NEIGHBORS_POINTKDTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/randomaccessrange.h"

#include "pastel/geometry/pointkdtree.h"

#include <vector>

namespace Pastel
{

	//! Counts the number of neighbors for all query points.
	/*!
	kdTree:
	A point kd-tree.

	querySet:
	A set of n object iterators to 'kdTree'. Denotes the set
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
	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator,
		typename Real_Iterator,
		typename NormBijection,
		typename Integer_OutputIterator>
	void countAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet,
		const NormBijection& normBijection,
		Integer_OutputIterator result);

}

#include "pastel/geometry/count_all_neighbors_pointkdtree.hpp"

#endif
