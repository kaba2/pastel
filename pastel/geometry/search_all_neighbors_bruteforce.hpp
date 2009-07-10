#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_BRUTEFORCE_HPP
#define PASTEL_SEARCH_ALL_NEIGHBORS_BRUTEFORCE_HPP

#include "pastel/geometry/search_all_neighbors_bruteforce.h"
#include "pastel/geometry/search_all_neighbors_1d.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/smallfixedset.h"
#include "pastel/sys/pastelomp.h"

#include <algorithm>

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
	void searchAllNeighborsBruteForce(
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

		const integer points = pointSet.size();

		if (points == 0 || kNearest == 0)
		{
			return;
		}
		
		const integer dimension = pointSet.front().dimension();

		if (dimension == 1)
		{
			searchAllNeighbors1d(
				pointSet,
				kNearest,
				maxDistance,
				normBijection,
				&nearestArray,
				0);
			
			return;
		}

		typedef Detail_AllNearestNeighborsBruteForce::Entry<Real> Entry;
		typedef SmallFixedSet<Entry> NearestSet;
		typedef typename NearestSet::iterator NearestIterator;

		// Due to rounding errors exact comparisons can miss
		// reporting some of the points, giving incorrect results.
		// For example, consider n > k points on a 2d circle and make a 
		// k-nearest query to its center. With bad luck the algorithm
		// can report less than k points. We avoid this behaviour
		// by scaling the culling radius up by a protective factor.
		const Real protectiveFactor = 
			normBijection.scalingFactor(1.001);

#pragma omp parallel
		{
			NearestSet nearestSet(kNearest);

#pragma omp for
		for (integer i = 0;i < points;++i)
		{
			const Point<N, Real>& iPoint = pointSet[i];

			Real cullDistance = maxDistance;
			nearestSet.clear();

			for (integer j = 0;j < i;++j)
			{
				const Real distance = distance2(pointSet[j], iPoint, 
					normBijection, cullDistance);

				if (distance <= cullDistance)
				{
					nearestSet.insert(Entry(distance, j));
					if (nearestSet.full())
					{
						cullDistance = std::min(
							nearestSet.back().distance_ * protectiveFactor,
							maxDistance);
					}
				}
			}

			for (integer j = i + 1;j < points;++j)
			{
				const Real distance = distance2(pointSet[j], iPoint, 
					normBijection, cullDistance);

				if (distance <= cullDistance)
				{
					nearestSet.insert(Entry(distance, j));
					if (nearestSet.full())
					{
						cullDistance = 
							std::min(
							nearestSet.back().distance_ * protectiveFactor,
							maxDistance);
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

}

#endif
