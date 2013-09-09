#ifndef PASTELGEOMETRY_SEARCH_ALL_NEIGHBORS_BRUTEFORCE_HPP
#define PASTELGEOMETRY_SEARCH_ALL_NEIGHBORS_BRUTEFORCE_HPP

#include "pastel/geometry/search_all_neighbors_bruteforce.h"
#include "pastel/geometry/search_all_neighbors_1d.h"
#include "pastel/geometry/distance_point_point.h"

#include <tbb/parallel_for.h>

#include <set>
#include <algorithm>

namespace Pastel
{

	namespace AllNearestNeighborsBruteForce_
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

	template <typename Point_RandomAccessRange,
		typename PointPolicy,
		typename Point_Iterator,
		typename Real_RandomAccessRange,
		typename Point_Iterator_RandomAccessRange,
		typename NormBijection>
	void searchAllNeighborsBruteForce(
		const Point_RandomAccessRange& pointSet,
		integer dimension,
		const PointPolicy& pointPolicy,
		Array<Point_Iterator>& nearestArray,
		integer kNearest,
		const Real_RandomAccessRange& maxDistanceSet,
		const Point_Iterator_RandomAccessRange& indexSet,
		const NormBijection& normBijection)
	{
		ENSURE_OP(dimension, >, 0);
		ENSURE_OP(kNearest, >=, 0);
		ENSURE_OP(nearestArray.width(), >=, kNearest);
		ENSURE_OP(nearestArray.height(), >=, indexSet.size());
		ENSURE_OP(maxDistanceSet.size(), ==, indexSet.size());

		if (kNearest > pointSet.size())
		{
			kNearest = pointSet.size();
		}

		const integer points = pointSet.size();
		const integer indices = indexSet.size();

		if (points == 0 || kNearest == 0 || indices == 0)
		{
			return;
		}
		
		typedef typename PointPolicy::Real Real;
		typedef AllNearestNeighborsBruteForce_::Entry<Real> Entry;
		typedef std::set<Entry> NearestSet;
		typedef typename NearestSet::iterator NearestIterator;
		typedef typename PointPolicy::ConstIterator RealIterator;

		using IndexRange = tbb::blocked_range<integer>;

		// Due to rounding errors exact comparisons can miss
		// reporting some of the points, giving incorrect results.
		// For example, consider n > k points on a 2d circle and make a 
		// k-nearest query to its center. With bad luck the algorithm
		// can report less than k points. We avoid this behaviour
		// by scaling the culling radius up by a protective factor.
		const Real protectiveFactor = 
			normBijection.scalingFactor(1.01);

		auto searchNeighbors = [&](const IndexRange& range)
		{
			NearestSet nearestSet;
			for (integer i = range.begin();i < range.end();++i)
			{
				RealIterator iPoint = pointPolicy.begin(*indexSet[i]);

				const Real maxDistance = maxDistanceSet[i];
				Real cullDistance = maxDistance;
				nearestSet.clear();

				for (integer j = 0;j < points;++j)
				{
					if (j != i)
					{
						RealIterator jPoint = pointPolicy.begin(pointSet[j]);

						const Real distance = 
							distance2(iPoint, jPoint, dimension,
							normBijection, cullDistance);

						if (distance < cullDistance)
						{
							nearestSet.insert(Entry(distance, j));
							if (nearestSet.size() > kNearest)
							{
								nearestSet.erase(
									std::prev(nearestSet.end()));
							}
							if (nearestSet.size() == kNearest)
							{
								cullDistance = std::min(
									std::prev(nearestSet.end())->distance_ * protectiveFactor,
									maxDistanceSet[i]);
							}
						}
					}
				}

				integer nearestIndex = 0;
				NearestIterator iter = nearestSet.begin();
				const NearestIterator iterEnd = nearestSet.end();
				while(iter != iterEnd)
				{
					nearestArray(nearestIndex, i) = pointSet.begin() + iter->index_;
					++nearestIndex;
					++iter;
				}

				ASSERT2(maxDistance != infinity<Real>() ||
					nearestIndex == kNearest, nearestIndex, kNearest);

				for (;nearestIndex < kNearest;++nearestIndex)
				{
					nearestArray(nearestIndex, i) = pointSet.end();
				}
			}
		};

		tbb::parallel_for(IndexRange(0, indices), 
			searchNeighbors);
	}

}

#endif
