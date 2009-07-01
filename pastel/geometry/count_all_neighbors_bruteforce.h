#ifndef PASTEL_COUNT_ALL_NEIGHBORS_BRUTEFORCE_H
#define PASTEL_COUNT_ALL_NEIGHBORS_BRUTEFORCE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	template <int N, typename Real, typename NormBijection>
	void countAllNeighborsBruteForce(
		const std::vector<Point<N, Real> >& pointSet,
		const std::vector<PASTEL_NO_DEDUCTION(Real)>& maxDistanceSet,
		const NormBijection& normBijection,
		std::vector<integer>& countSet);

}

#include "pastel/geometry/count_all_neighbors_bruteforce.hpp"

#endif
