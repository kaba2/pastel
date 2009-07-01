#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_1D_HPP
#define PASTEL_SEARCH_ALL_NEIGHBORS_1D_HPP

#include "pastel/geometry/search_all_neighbors_1d.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/smallfixedset.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	namespace Detail_SearchAllNeighbors1d
	{

		template <typename Real, typename ConstIterator>
		void assignNearest(
			const ConstIterator& begin,
			const ConstIterator& end,
			const ConstIterator& iter,
			integer searchIndex,
			Array<2, integer>& nearestArray)
		{
			ConstIterator leftIter = iter;
			ConstIterator rightIter = iter;

			integer nearestIndex = 0;

			const Real position = iter->key();

			while(leftIter != begin && rightIter + 1 != end)
			{
				const Real leftDistance = 
					position - (leftIter - 1)->key();
				
				const Real rightDistance =
					(rightIter + 1)->key() - position;

				if (leftDistance < rightDistance)
				{
					--leftIter;
					nearestArray(nearestIndex, searchIndex) =
						leftIter->value();
					++nearestIndex;
				}
				else
				{
					++rightIter;
					nearestArray(nearestIndex, searchIndex) =
						rightIter->value();
					++nearestIndex;
				}
			}

			while(leftIter != begin)
			{
				--leftIter;
				nearestArray(nearestIndex, searchIndex) =
					leftIter->value();
				++nearestIndex;
			}

			while(rightIter + 1 != end)
			{
				++rightIter;
				nearestArray(nearestIndex, searchIndex) =
					rightIter->value();
				++nearestIndex;
			}
		}

	}

	template <int N, typename Real, typename NormBijection>
	void searchAllNeighbors1d(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		Array<2, integer>& nearestArray)
	{
		const integer points = pointSet.size();

		if (points == 0)
		{
			return;
		}

		const integer dimension = pointSet.front().dimension();

		ENSURE1(dimension == 1, dimension);

		typedef std::vector<KeyValue<Real, integer> > SearchSet;
		typedef typename SearchSet::iterator Iterator;
		typedef typename SearchSet::const_iterator ConstIterator;
		
		SearchSet searchSet;
		searchSet.reserve(points);
		
		for (integer i = 0;i < points;++i)
		{
			searchSet.push_back(
				keyValue(pointSet[i][0], i));
		}

		std::sort(searchSet.begin(), searchSet.end());

		const ConstIterator begin = searchSet.begin();
		const ConstIterator end = searchSet.end();

		if (maxDistance == infinity<Real>())
		{
			SmallFixedSet<KeyValue<Real, integer> > neighborSet(kNearest);

			ConstIterator iter = begin;
			ConstIterator leftIter = begin;
			ConstIterator rightIter = leftIter + kNearest + 1;

			while(iter != end)
			{
				const Real position = iter->key();

				Real farthestDistance = 
					std::max(
					(rightIter - 1)->key() - position,
					position - leftIter->key());

				// Slide left.
				while(leftIter != begin && rightIter - 1 != iter)
				{
					const Real distance = 
						position - (leftIter - 1)->key();
					if (distance >= farthestDistance)
					{
						break;
					}

					--leftIter;
					--rightIter;

					farthestDistance = 
						std::max(
						(rightIter - 1)->key() - position,
						distance);
				}

				// Slide right.
				while(leftIter != iter && rightIter != end)
				{
					const Real distance = 
						rightIter->key() - position;
					if (distance >= farthestDistance)
					{
						break;
					}

					++leftIter;
					++rightIter;

					farthestDistance = 
						std::max(
						distance,
						position - leftIter->key());
				}

				Detail_SearchAllNeighbors1d::assignNearest<Real>(
					leftIter, rightIter, iter, iter->value(),
					nearestArray);
				
				if (rightIter != end)
				{
					++leftIter;
					++rightIter;
				}
				++iter;
			}
		}
		else
		{
			SmallFixedSet<KeyValue<Real, integer> > neighborSet(kNearest);

			ConstIterator iter = begin;
			const Real radius = normBijection.toNorm(maxDistance);
#pragma omp parallel for
			for (integer i = 0;i < points;++i)
			{
				const ConstIterator iter = begin + i;

				const integer index = iter->value();
				const Real position = iter->key();

				const Real left = position - radius;
				const Real right = position + radius;
			
				const ConstIterator leftIter = std::lower_bound(
					searchSet.begin(), searchSet.end(), keyValue(left, index));
				const ConstIterator rightIter = std::upper_bound(
					searchSet.begin(), searchSet.end(), keyValue(right, index));

				Detail_SearchAllNeighbors1d::assignNearest<Real>(
					leftIter, rightIter, iter, iter->value(),
					nearestArray);
			}
		}
	}

}

#endif
