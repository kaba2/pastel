#ifndef PASTELGEOMETRY_COUNT_ALL_NEIGHBORS_BRUTEFORCE_HPP
#define PASTELGEOMETRY_COUNT_ALL_NEIGHBORS_BRUTEFORCE_HPP

#include "pastel/geometry/count_all_neighbors_bruteforce.h"
#include "pastel/geometry/count_all_neighbors_1d.h"

#include "pastel/sys/ensure.h"

#include "pastel/geometry/distance_point_point.h"

#include <tbb/parallel_for.h>

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
		const integer queries = indexEnd - indexBegin;

		if (points == 0 || queries == 0)
		{
			return;
		}

		const integer dimension = pointSet.front().n();

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

		using IndexRange = tbb::blocked_range<integer>;

		auto fillWithZeros = [&](const IndexRange& range)
		{
			std::fill(
				neighborsBegin + range.begin(), 
				neighborsBegin + range.end(), 0);
		};

		tbb::parallel_for(IndexRange(0, queries), 
			fillWithZeros)

		auto countNeighbors = [&](const IndexRange& range)
		{
			for (integer i = range.begin();i < range.end();++i)
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
		};

		tbb::parallel_for(IndexRange(0, queries),
			countNeighbors);
	}

}

#endif
