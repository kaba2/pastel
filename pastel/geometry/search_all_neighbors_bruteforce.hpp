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

	template <typename Object_Iterator,
		typename PointPolicy,
		typename Real_Iterator,
		typename Object_Iterator_Iterator,
		typename NormBijection>
	void searchAllNeighborsBruteForce(
		const RandomAccessRange<Object_Iterator>& pointSet,
		integer dimension,
		const PointPolicy& pointPolicy,
		Array<Object_Iterator>& nearestArray,
		integer kNearest,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet,
		const RandomAccessRange<Object_Iterator_Iterator>& indexSet,
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
		typedef Detail_AllNearestNeighborsBruteForce::Entry<Real> Entry;
		typedef SmallFixedSet<Entry> NearestSet;
		typedef typename NearestSet::iterator NearestIterator;
		typedef typename PointPolicy::ConstIterator RealIterator;

		// Due to rounding errors exact comparisons can miss
		// reporting some of the points, giving incorrect results.
		// For example, consider n > k points on a 2d circle and make a 
		// k-nearest query to its center. With bad luck the algorithm
		// can report less than k points. We avoid this behaviour
		// by scaling the culling radius up by a protective factor.
		const Real protectiveFactor = 
			normBijection.scalingFactor(1.01);

#		pragma omp parallel
		{
			NearestSet nearestSet(kNearest);

#		pragma omp for
		for (integer i = 0;i < indices;++i)
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
						if (nearestSet.full())
						{
							cullDistance = std::min(
								nearestSet.back().distance_ * protectiveFactor,
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
		}
	}

}

#endif
