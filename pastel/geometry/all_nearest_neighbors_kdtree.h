#ifndef PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_KDTREE_H
#define PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_KDTREE_H

#include "pastel/sys/point.h"
#include "pastel/sys/array.h"

#include <vector>

namespace Pastel
{

	//! Finds all-k-nearest-neighbours of a point set.
	/*!
	Preconditions:
	kNearestBegin >= 0
	kNearestEnd < pointSet.size()
	kNearestBegin <= kNearestEnd
	maxDistance >= 0
	maxRelativeError >= 0
	
	Time complexity:
	?

	Algorithm:
	kd-tree
	*/

	template <int N, typename Real, typename NormBijection>
	void allNearestNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearestBegin,
		integer kNearestEnd,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		Array<2, integer>* nearestArray,
		Array<2, Real>* distanceArray = 0);

}

#include "pastel/geometry/all_nearest_neighbors_kdtree.hpp"

#endif
