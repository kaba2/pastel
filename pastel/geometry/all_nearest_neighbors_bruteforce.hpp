#ifndef PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_BRUTEFORCE_HPP
#define PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_BRUTEFORCE_HPP

#include "pastel/geometry/all_nearest_neighbors_bruteforce.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/smallfixedset.h"

#include <omp.h>

namespace Pastel
{

	namespace Detail_AllNearestNeighborsBruteForce
	{

		template <typename Real>
		class Entry
		{
		public:
			Entry(const Real& distance,
				integer index)
				: distance_(distance)
				, index_(index)
			{
			}

			bool operator<(const Entry& that) const
			{
				if (distance_ < that.distance_)
				{
					return true;
				}
				if (that.distance_ < distance_)
				{
					return false;
				}
				return index_ < that.index_;
			}

			Real distance_;
			integer index_;
		};

	}

	template <int N, typename Real, typename NormBijection>
	void allNearestNeighborsBruteForce(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		Array<2, integer>& nearestArray)
	{
		ENSURE1(kNearest >= 1, kNearest);
		ENSURE2(kNearest < pointSet.size(), kNearest, pointSet.size());
		ENSURE(maxDistance >= 0);
		ENSURE2(nearestArray.width() == kNearest, nearestArray.width(), kNearest);
		ENSURE2(nearestArray.height() == pointSet.size(), nearestArray.height(), pointSet.size());

		typedef Detail_AllNearestNeighborsBruteForce::Entry<Real> Entry;
		typedef SmallFixedSet<Entry> NearestSet;
		typedef typename NearestSet::iterator NearestIterator;

		// The commented OpenMp pragmas represent
		// another way to parallelize the searching.
		// I timed the performances and noted that
		// parallelizing the outer loop is much faster
		// (presumably because it does not need a critical
		// section and also parallelizes writing the results).

		const integer dimension = pointSet.front().dimension();
		const integer points = pointSet.size();
#pragma omp parallel for
		for (integer i = 0;i < points;++i)
		{
			const Point<N, Real>& iPoint = pointSet[i];

			NearestSet nearestSet(kNearest);

			real cullDistance = maxDistance;

//#pragma omp parallel for
			for (integer j = 0;j < points;++j)
			{
				if (j != i)
				{
					const real distance = distance2(pointSet[j], iPoint, 
						normBijection, cullDistance);

					if (distance <= cullDistance)
					{
//#pragma omp critical
						{
							nearestSet.insert(Entry(distance, j));
							if (nearestSet.full())
							{
								cullDistance = nearestSet.back().distance_;
							}
						}
					}
				}
			}

			integer nearestIndex = 0;
			NearestIterator iter = nearestSet.begin();
			const NearestIterator iterEnd = nearestSet.end();
			while(iter != iterEnd)
			{
				nearestArray(nearestIndex, i) = iter->index_;
				++nearestIndex;
				++iter;
			}

			ASSERT2(maxDistance != infinity<Real>() ||
				nearestIndex == kNearest, nearestIndex, kNearest);

			for (;nearestIndex < kNearest;++nearestIndex)
			{
				nearestArray(nearestIndex, i) = -1;
			}
		}
	}

}

#endif
