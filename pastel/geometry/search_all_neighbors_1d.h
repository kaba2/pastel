// Description: Nearest neighbor searching in 1D
// Detail: Implements an optimal algorithm that only works in 1D
// Documentation: nearest_neighbors.txt

#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_1D_H
#define PASTEL_SEARCH_ALL_NEIGHBORS_1D_H

#include "pastel/sys/array.h"
#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	//! Nearest neighbor searching in 1D.
	/*!
	Preconditions:
	kNearest >= 0
	maxDistance >= 0

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
	Note: Can be given a null pointer in which case
	reporting this information is skipped.

	distanceArray (output):
	An array of distances such that d(i, j) contains
	the distance between i:th point in the index set
	and its j:th neighbor.
	Note: Can be given a null pointer in which case
	reporting this information is skipped.
	*/

	template <int N, typename Real, typename NormBijection,
	typename ConstIndexIterator>
	void searchAllNeighbors1d(
		const std::vector<Point<N, Real> >& pointSet,
		const ConstIndexIterator& indexSetBegin,
		const ConstIndexIterator& indexSetEnd,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		Array<2, integer>* nearestArray,
		Array<2, PASTEL_NO_DEDUCTION(Real)>* distanceArray);

}

#include "pastel/geometry/search_all_neighbors_1d.hpp"

#endif
