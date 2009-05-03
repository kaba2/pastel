#ifndef PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_MORE_HPP
#define PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_MORE_HPP

#include "pastel/geometry/all_nearest_neighbors.h"

#include <omp.h>

#include <set>

namespace Pastel
{

	namespace Detail_AllNearestNeighborsNaive
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
	void allNearestNeighborsNaive(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		Array<2, integer>& nearestSet)
	{
		ENSURE1(kNearest >= 1, kNearest);
		ENSURE(maxDistance >= 0);
		ENSURE2(nearestSet.width() == kNearest, nearestSet.width(), kNearest);
		ENSURE2(nearestSet.height() == pointSet.size(), nearestSet.height(), pointSet.size());

		typedef Detail_AllNearestNeighborsNaive::Entry<Real> Entry;
		typedef std::set<Entry> NearestSet;
		typedef typename NearestSet::iterator NearestIterator;

		const integer points = pointSet.size();
		for (integer i = 0;i < points;++i)
		{
			const Point<N, Real>& iPoint = pointSet[i];

			NearestSet nearest;

			real cullDistance = maxDistance;

#pragma omp parallel for
			for (integer j = 0;j < points;++j)
			{
				if (j != i)
				{
					//const real distance = normBijection(pointSet[j] - iPoint);
					const real distance = normBijection(pointSet[j] - iPoint, cullDistance);
					if (distance <= cullDistance)
					{
#pragma omp critical
						{
							nearest.insert(Entry(distance, j));

							NearestIterator lastIter = nearest.end();
							--lastIter;
							if (nearest.size() > kNearest)
							{
								nearest.erase(lastIter);
							}

							lastIter = nearest.end();
							--lastIter;
							cullDistance = lastIter->distance_;
						}
					}
				}
			}

			integer nearestIndex = 0;
			NearestIterator iter = nearest.begin();
			const NearestIterator iterEnd = nearest.end();
			while(iter != iterEnd)
			{
				nearestSet(nearestIndex, i) = iter->index_;
				++nearestIndex;
				++iter;
			}

			for (;nearestIndex < kNearest;++nearestIndex)
			{
				nearestSet(nearestIndex, i) = -1;
			}
		}
	}

}

#endif
