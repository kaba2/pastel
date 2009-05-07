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
		Array<2, integer>& nearestArray)
	{
		ENSURE1(kNearest >= 1, kNearest);
		ENSURE2(kNearest < pointSet.size(), kNearest, pointSet.size());
		ENSURE(maxDistance >= 0);
		ENSURE2(nearestArray.width() == kNearest, nearestArray.width(), kNearest);
		ENSURE2(nearestArray.height() == pointSet.size(), nearestArray.height(), pointSet.size());

		typedef Detail_AllNearestNeighborsNaive::Entry<Real> Entry;
		//typedef std::set<Entry> NearestSet;
		typedef SmallFixedSet<Entry> NearestSet;
		typedef typename NearestSet::iterator NearestIterator;

		const integer points = pointSet.size();
		for (integer i = 0;i < points;++i)
		{
			const Point<N, Real>& iPoint = pointSet[i];

			//NearestSet nearestSet;
			NearestSet nearestSet(kNearest);

			real cullDistance = maxDistance;

#pragma omp parallel for
			for (integer j = 0;j < points;++j)
			{
				if (j != i)
				{
					const real distance = 
						normBijection.compute(pointSet[j] - iPoint, cullDistance);
					if (distance <= cullDistance)
					{
#pragma omp critical
						{
							nearestSet.insert(Entry(distance, j));
							if (nearestSet.full())
							{
								cullDistance = nearestSet.back().distance_;
							}

							/*
							if (nearestSet.size() > kNearest)
							{
								NearestIterator lastIter = nearestSet.end();
								--lastIter;
								nearestSet.erase(lastIter);

								// Since we now know k neighboring points,
								// we can bound the search radius
								// by the distance to the currently farthest
								// neighbor.

								lastIter = nearestSet.end();
								--lastIter;
								cullDistance = lastIter->distance_;
							}
							*/
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
