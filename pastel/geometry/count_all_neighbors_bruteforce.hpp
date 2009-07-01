#ifndef PASTEL_COUNT_ALL_NEIGHBORS_BRUTEFORCE_HPP
#define PASTEL_COUNT_ALL_NEIGHBORS_BRUTEFORCE_HPP

#include "pastel/geometry/count_all_neighbors_bruteforce.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/ensure.h"

#include "pastel/geometry/distance_point_point.h"

#include <algorithm>

namespace Pastel
{

	template <int N, typename Real, typename NormBijection>
	void countAllNeighborsBruteForce(
		const std::vector<Point<N, Real> >& pointSet,
		const std::vector<PASTEL_NO_DEDUCTION(Real)>& maxDistanceSet,
		const NormBijection& normBijection,
		std::vector<integer>& countSet)
	{
		ENSURE2(pointSet.size() == maxDistanceSet.size(),
			pointSet.size(), maxDistanceSet.size());

		const integer points = pointSet.size();
				
		countSet.resize(points);
		std::fill(countSet.begin(), countSet.end(), 0);

#pragma omp parallel for
		for (integer i = 0;i < points;++i)
		{
			const Real maxDistance = maxDistanceSet[i];
			for (integer j = 0;j < points;++j)
			{
				if (distance2(pointSet[i], pointSet[j], normBijection) <= maxDistance)
				{
					if (i != j)
					{
						++countSet[i];
					}
				}
			}
		}
	}


}

#endif
