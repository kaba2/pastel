#ifndef PASTEL_ALL_NEAREST_NEIGHBORS_OWN_H
#define PASTEL_ALL_NEAREST_NEIGHBORS_OWN_H

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

	Time complexity:
	Unknown
	Could it be O(d^2 log(d) n sqrt(n))?

	Algorithm:
	Expanding neighborhood via sorted axis projections.
	*/

	template <int N, typename Real>
	void allNearestNeighborsOwn(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		Array<2, integer>& nearestSet);

}

#include "pastel/geometry/all_nearest_neighbors_own.hpp"

#endif
