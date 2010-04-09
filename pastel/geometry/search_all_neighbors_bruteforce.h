// Description: All-k-nearest-neighbors searching using a brute-force algorithm

#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_BRUTEFORCE_H
#define PASTEL_SEARCH_ALL_NEIGHBORS_BRUTEFORCE_H

#include "pastel/sys/vector.h"
#include "pastel/sys/array.h"

#include <vector>

namespace Pastel
{

	//! Finds all-k-nearest-neighbours of a point set.
	/*!
	Preconditions:
	kNearest >= 0
	maxDistance >= 0

	Time complexity:
	O(d n^2 log k)
	where
	d is the dimension
	n = pointSet.size()

	pointSet:
	The set of points to do the searching in.

	dimension:
	Dimension of the point-set.

	pointPolicy:
	An object modeling the PointPolicy concept.
	See pointpolicy.txt.

	nearestArray (output):
	An array of iterators from 'pointSet' such that 
	a(i, j) contains the j:th neighbor of the i:th point 
	in the 'indexSet'.

	kNearest:
	The number of nearest neighbors to seek for.

	maxDistance:
	A distance after which points aren't considered
	neighbors. This distance is in terms of the
	norm bijection. Note: Can be set to infinity.

	indexSet:
	A sequence of integers denoting iterators in 'pointSet'
	for which the neighbors are sought for. Note: use the 
	CountingIterator class for generating a range of iterator
	values without using any additional storage.

	normBijection:
	The norm bijection to use to define distance.
	See 'pastel/math/normbijection.txt'.
	*/

	template <typename Object_Iterator,
		typename PointPolicy,
		typename Real_Iterator,
		typename Object_Iterator_Iterator,
		typename NormBijection>
	void searchAllNeighborsBruteForce(
		const RandomAccessRange<Object_Iterator>& pointSet,
		integer dimension,
		const PointPolicy& pointPolicy,
		Array<Object_Iterator>& nearestArray,
		integer kNearest,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet,
		const RandomAccessRange<Object_Iterator_Iterator>& indexSet,
		const NormBijection& normBijection);

}

#include "pastel/geometry/search_all_neighbors_bruteforce.hpp"

#endif
