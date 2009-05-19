#ifndef PASTEL_COUNT_ALL_NEAREST_NEIGHBORS_KDTREE_H
#define PASTEL_COUNT_ALL_NEAREST_NEIGHBORS_KDTREE_H

#include "pastel/sys/point.h"
#include "pastel/sys/mytypes.h"

#include <vector>

namespace Pastel
{

	//! Counts nearest neighbours of a point set.
	/*!
	Preconditions:
	maxDistance >= 0
	maxRelativeError >= 0
	
	Time complexity:
	?

	Algorithm:
	kd-tree
	*/

	template <int N, typename Real, typename NormBijection>
	void countAllNearestNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		std::vector<integer>& countSet);

	//! Counts nearest neighbours of a point set.
	/*!
	Preconditions:
	maxDistance >= 0
	maxRelativeError >= 0
	
	Time complexity:
	?

	Algorithm:
	kd-tree
	*/

	template <int N, typename Real, typename NormBijection>
	void countAllNearestNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		const std::vector<PASTEL_NO_DEDUCTION(Real)>& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		std::vector<integer>& countSet);

}

#include "pastel/geometry/count_all_nearest_neighbors_kdtree.hpp"

#endif
