// Description: Nearest neighbor searching using a brute-force algorithm
// Documentation: nearest_neighbors.txt

#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_BRUTEFORCE_H
#define PASTEL_SEARCH_ALL_NEIGHBORS_BRUTEFORCE_H

#include "pastel/sys/point.h"
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

	indexSetBegin, indexSetEnd:
	A sequence of integers denoting indices of those
	points in the 'pointSet' for which the neighbors
	are sought for. Note: use the CountingIterator 
	class for generating a range of integer values 
	without using any additional storage.

	kNearest:
	The number of nearest neighbors to seek for.

	maxDistance:
	A distance after which points aren't considered
	neighbors. This distance is in terms of the
	norm bijection. Note: Can be set to infinity.

	normBijection:
	The norm bijection to use to define distance.

	nearestArray (output):
	An array of indices such that a(i, j) contains
	the j:th neighbor of the i:th point in the index set.
	*/

	template <int N, typename Real, typename NormBijection,
	typename ConstIndexIterator>
	void searchAllNeighborsBruteForce(
		const std::vector<Point<Real, N> >& pointSet,
		const ConstIndexIterator& indexSetBegin,
		const ConstIndexIterator& indexSetEnd,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		Array<integer, 2>& nearestArray);

}

#include "pastel/geometry/search_all_neighbors_bruteforce.hpp"

#endif
