// Description: All-k-nearest-neighbors searching using a brute-force algorithm

#ifndef PASTELGEOMETRY_SEARCH_ALL_NEIGHBORS_BRUTEFORCE_H
#define PASTELGEOMETRY_SEARCH_ALL_NEIGHBORS_BRUTEFORCE_H

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

	locator:
	An object modeling the Locator concept.
	See pointpolicies.txt.

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

	template <typename Point_RandomAccessRange,
		typename Locator,
		typename Point_Iterator,
		typename Real_RandomAccessRange,
		typename Point_Iterator_RandomAccessRange,
		typename NormBijection>
	void searchAllNeighborsBruteForce(
		const Point_RandomAccessRange& pointSet,
		integer dimension,
		const Locator& locator,
		Array<Point_Iterator>& nearestArray,
		integer kNearest,
		const Real_RandomAccessRange& maxDistanceSet,
		const Point_Iterator_RandomAccessRange& indexSet,
		const NormBijection& normBijection);

}

#include "pastel/geometry/search_all_neighbors_bruteforce.hpp"

#endif
