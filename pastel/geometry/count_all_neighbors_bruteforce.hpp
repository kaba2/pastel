#ifndef PASTELGEOMETRY_COUNT_ALL_NEIGHBORS_BRUTEFORCE_HPP
#define PASTELGEOMETRY_COUNT_ALL_NEIGHBORS_BRUTEFORCE_HPP

#include "pastel/geometry/count_all_neighbors_bruteforce.h"
#include "pastel/geometry/count_all_neighbors_1d.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/ensure.h"

#include "pastel/geometry/distance_point_point.h"

#include <algorithm>

namespace Pastel
{

	template <typename Real, int N, typename NormBijection,
	typename ConstIndexIterator, typename ConstDistanceIterator,
	typename CountIterator>
	void countAllNeighborsBruteForce(
		const std::vector<Vector<Real, N> >& pointSet,
		const ConstIndexIterator& indexBegin,
		const ConstIndexIterator& indexEnd,
		const ConstDistanceIterator& maxDistanceBegin,
		const NormBijection& normBijection,
		const CountIterator& neighborsBegin)
	{
		const integer points = pointSet.size();
		const integer indices = indexEnd - indexBegin;

		if (points == 0 || indices == 0)
		{
			return;
		}

		const integer dimension = pointSet.front().dimension();

		if (dimension == 1)
		{
			countAllNeighbors1d(
				pointSet, 
				indexBegin, 
				indexEnd,
				maxDistanceBegin,
				normBijection, 
				neighborsBegin);

			return;
		}

		std::fill(neighborsBegin, neighborsBegin + indices, 0);

#		pragma omp parallel for
		for (integer i = 0;i < indices;++i)
		{
			const integer index = indexBegin[i];
			const Vector<Real, N>& iPoint = pointSet[index];
			const Real maxDistance = maxDistanceBegin[i];
			for (integer j = 0;j < points;++j)
			{
				if (distance2(iPoint, pointSet[j], normBijection) < maxDistance)
				{
					++neighborsBegin[i];
				}
			}
		}
	}


}

#endif
