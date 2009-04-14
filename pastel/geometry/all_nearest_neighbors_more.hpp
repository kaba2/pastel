#ifndef PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_MORE_HPP
#define PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_MORE_HPP

#include "pastel/geometry/all_nearest_neighbors.h"

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

		const integer points = pointSet.size();
		for (integer i = 0;i < points;++i)
		{
			const Point<N, Real>& iPoint = pointSet[i];
			
			typedef std::set<Entry> NearestSet;
			typedef typename NearestSet::iterator NearestIterator;

			NearestSet nearest;

			integer j = 0;

			while(nearest.size() < kNearest && j < points)
			{
				if (j != i)
				{
					const real distance = normBijection(pointSet[j] - iPoint);
					if (distance <= maxDistance)
					{
						nearest.insert(Entry(distance, j));
					}
				}
				++j;
			}

			ASSERT2(nearest.size() == kNearest, nearest.size(), kNearest);

			NearestIterator lastIter = nearest.end();
			--lastIter;

			real cullDistance = lastIter->distance_;

			while(j < points)
			{
				if (j != i)
				{
					const real distance = normBijection(pointSet[j] - iPoint);
					if (distance <= cullDistance)
					{
						nearest.erase(lastIter);
						nearest.insert(Entry(distance, j));
						lastIter = nearest.end();
						--lastIter;
						cullDistance = lastIter->distance_;
					}
				}
				++j;
			}

			ASSERT2(nearest.size() == kNearest, nearest.size(), kNearest);

			integer nearestIndex = 0;
			NearestIterator iter = nearest.begin();
			const NearestIterator iterEnd = nearest.end();
			while(iter != iterEnd)
			{
				nearestSet(nearestIndex, i) = iter->index_;
				++nearestIndex;
				++iter;
			}
		}
	}

}

#endif
