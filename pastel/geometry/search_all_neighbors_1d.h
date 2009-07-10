#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_1D_H
#define PASTEL_SEARCH_ALL_NEIGHBORS_1D_H

#include "pastel/sys/array.h"
#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	template <int N, typename Real, typename NormBijection>
	void searchAllNeighborsBruteForce(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		Array<2, integer>* nearestArray,
		Array<2, PASTEL_NO_DEDUCTION(Real)>* distanceArray);

}

#include "pastel/geometry/search_all_neighbors_1d.hpp"

#endif
