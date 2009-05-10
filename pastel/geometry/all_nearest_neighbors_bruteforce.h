#ifndef PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_BRUTEFORCE_H
#define PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_BRUTEFORCE_H

#include "pastel/sys/point.h"
#include "pastel/sys/array.h"

#include <vector>

namespace Pastel
{

	//! Finds all-k-nearest-neighbours of a point set.
	/*!
	Preconditions:
	kNearest >= 1
	kNearest <= pointSet.size()
	maxDistance >= 0

	Time complexity:
	O(d n^2 log k)
	where
	d is the dimension
	n = pointSet.size()

	Algorithm:
	Linear scan (brute force).
	*/

	template <int N, typename Real, typename NormBijection>
	void allNearestNeighborsBruteForce(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		Array<2, integer>& nearestArray);

}

#include "pastel/geometry/all_nearest_neighbors_bruteforce.hpp"

#endif
