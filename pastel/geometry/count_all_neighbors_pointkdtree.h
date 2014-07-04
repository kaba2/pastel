// Description: All-nearest-neighbors counting using PointKdTree

#ifndef PASTELGEOMETRY_COUNT_ALL_NEIGHBORS_POINTKDTREE_H
#define PASTELGEOMETRY_COUNT_ALL_NEIGHBORS_POINTKDTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/range.h"

#include "pastel/math/euclidean_normbijection.h"

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
	template <
		typename Settings, template <typename> class Customization,
		typename Point_ConstIterator_RandomAccessRange,
		typename Real_RandomAccessRange,
		typename Integer_OutputIterator,
		typename NormBijection = Euclidean_NormBijection<Real>>
	void countAllNeighbors(
		const PointKdTree<Settings, Customization>& kdTree,
		const Point_ConstIterator_RandomAccessRange& querySet,
		const Real_RandomAccessRange& maxDistanceSet,
		Integer_OutputIterator result,
		integer bucketSize = 8,
		const NormBijection& normBijection = NormBijection());

}

#include "pastel/geometry/count_all_neighbors_pointkdtree.hpp"

#endif
