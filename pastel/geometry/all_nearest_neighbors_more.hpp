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
			Entry(const Real& distance2,
				integer index)
				: distance2_(distance2)
				, index_(index)
			{
			}

			bool operator<(const Entry& that) const
			{
				if (distance2_ < that.distance2_)
				{
					return true;
				}
				if (that.distance2_ < distance2_)
				{
					return false;
				}
				return index_ < that.index_;
			}

			Real distance2_;
			integer index_;
		};

	}

	template <int N, typename Real>
	void allNearestNeighborsNaive(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		Array<2, integer>& nearestSet)
	{
		ENSURE1(kNearest >= 1, kNearest);
		ENSURE2(nearestSet.width() == kNearest, nearestSet.width(), kNearest);
		ENSURE2(nearestSet.height() == pointSet.size(), nearestSet.height(), pointSet.size());

		typedef Detail_AllNearestNeighborsNaive::Entry<Real> Entry;

		const Real maxDistance2 = square(maxDistance);

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
					const real distance2 = dot(pointSet[j] - iPoint);
					if (distance2 <= maxDistance2)
					{
						nearest.insert(Entry(distance2, j));
					}
				}
				++j;
			}

			ASSERT2(nearest.size() == kNearest, nearest.size(), kNearest);

			NearestIterator lastIter = nearest.end();
			--lastIter;

			real cullDistance2 = lastIter->distance2_;

			while(j < points)
			{
				if (j != i)
				{
					const real distance2 = dot(pointSet[j] - iPoint);
					if (distance2 <= cullDistance2)
					{
						nearest.erase(lastIter);
						nearest.insert(Entry(distance2, j));
						lastIter = nearest.end();
						--lastIter;
						cullDistance2 = lastIter->distance2_;
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
