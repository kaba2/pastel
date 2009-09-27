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

		template <typename Real, typename ConstIterator, typename NormBijection>
		void assignNearest(
			const ConstIterator& begin,
			const ConstIterator& end,
			const ConstIterator& iter,
			integer searchIndex,
			integer kNearest,
			const NormBijection& normBijection,
			Array<integer, 2>* nearestArray,
			Array<Real, 2>* distanceArray)
		{
			ConstIterator leftIter = iter;
			ConstIterator rightIter = iter;

			integer nearestIndex = 0;

			const Real position = iter->key();

			while(leftIter != begin && rightIter + 1 != end && nearestIndex < kNearest)
			{
				const Real leftDistance = 
					position - (leftIter - 1)->key();
				
				const Real rightDistance =
					(rightIter + 1)->key() - position;

				if (leftDistance < rightDistance)
				{
					--leftIter;
					if (nearestArray)
					{
						(*nearestArray)(nearestIndex, searchIndex) =
							leftIter->value();
					}
					if (distanceArray)
					{
						(*distanceArray)(nearestIndex, searchIndex) =
							normBijection.toBijection(leftDistance);
					}
					++nearestIndex;
				}
				else
				{
					++rightIter;
					if (nearestArray)
					{
						(*nearestArray)(nearestIndex, searchIndex) =
							rightIter->value();
					}
					if (distanceArray)
					{
						(*distanceArray)(nearestIndex, searchIndex) =
							normBijection.toBijection(rightDistance);
					}
					++nearestIndex;
				}
			}

			while(leftIter != begin && nearestIndex < kNearest)
			{
				--leftIter;
				if (nearestArray)
				{
					(*nearestArray)(nearestIndex, searchIndex) =
						leftIter->value();
				}
				if (distanceArray)
				{
					(*distanceArray)(nearestIndex, searchIndex) =
						normBijection.toBijection(position - leftIter->key());
				}
				++nearestIndex;
			}

			while(rightIter + 1 != end && nearestIndex < kNearest)
			{
				++rightIter;
				if (nearestArray)
				{
					(*nearestArray)(nearestIndex, searchIndex) =
						rightIter->value();
				}
				if (distanceArray)
				{
					(*distanceArray)(nearestIndex, searchIndex) =
						normBijection.toBijection(rightIter->key() - position);
				}
				++nearestIndex;
			}
		}

	}

	template <typename Real, int N, typename NormBijection,
	typename ConstIndexIterator>
	void searchAllNeighbors1d(
		const std::vector<Vector<Real, N> >& pointSet,
		const ConstIndexIterator& indexSetBegin,
		const ConstIndexIterator& indexSetEnd,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		Array<integer, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray)
	{
		ENSURE_OP(kNearest, >=, 0);
		ENSURE_OP(maxDistance, >=, 0);

		if (kNearest > pointSet.size())
		{
			kNearest = pointSet.size();
		}

		const integer points = pointSet.size();
		const integer indices = indexSetEnd - indexSetBegin;

		if (indices == 0 || kNearest == 0 || indices == 0 ||
			(!nearestArray && !distanceArray))
		{
			return;
		}

		const integer dimension = pointSet.front().dimension();

		ENSURE_OP(dimension, ==, 1);

		typedef std::vector<KeyValue<Real, integer> > SearchSet;
		typedef typename SearchSet::iterator Iterator;
		typedef typename SearchSet::const_iterator ConstIterator;
		
		SearchSet searchSet;
		searchSet.reserve(points);
		
		for (integer i = 0;i < points;++i)
		{
			searchSet.push_back(keyValue(pointSet[i][0], i));
		}

		std::sort(searchSet.begin(), searchSet.end());

		const ConstIterator begin = searchSet.begin();
		const ConstIterator end = searchSet.end();

		if (maxDistance < infinity<Real>())
		{
			const Real radius = normBijection.toNorm(maxDistance);
#			pragma omp parallel for
			for (integer i = 0;i < indices;++i)
			{
				const integer index = indexSetBegin[i];
				const Real position = pointSet[index][0];
		
				const ConstIterator leftIter = std::lower_bound(
					begin, end, keyValue(position - radius, index));
				const ConstIterator rightIter = std::upper_bound(
					begin, end, keyValue(position + radius, index));
				const ConstIterator iter = std::find(
					leftIter, rightIter, keyValue(position, index));
				ASSERT(iter != rightIter);

				Detail_SearchAllNeighbors1d::assignNearest<Real>(
					leftIter, rightIter, iter, i, kNearest,
					normBijection,
					nearestArray, distanceArray);
			}
		}
		else
		{
#			pragma omp parallel for
			for (integer i = 0;i < indices;++i)
			{
				const integer index = indexSetBegin[i];
				const Real position = pointSet[index][0];
		
				const ConstIterator iter = std::find(
					begin, end, keyValue(position, index));
				ASSERT(iter != end);

				Detail_SearchAllNeighbors1d::assignNearest<Real>(
					begin, end, iter, i, kNearest,
					normBijection,
					nearestArray, distanceArray);
			}
		}
	}

	/*
	template <typename Real, int N, typename NormBijection>
	void searchAllNeighbors1d(
		const std::vector<Vector<Real, N> >& pointSet,
		integer kNearest,
		const NormBijection& normBijection,
		Array<integer, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray)
	{
		ENSURE_OP(kNearest, >=, 0);

		if (kNearest > pointSet.size())
		{
			kNearest = pointSet.size();
		}

		// This function assumes that the neighbors are sought
		// for all points in the set, and with no culling.
		// This is a very fast algorithm, but it does not
		// parallelize.

		const integer points = pointSet.size();

		if (indices == 0 || kNearest == 0 ||
			(!nearestArray && !distanceArray))
		{
			return;
		}

		const integer dimension = pointSet.front().dimension();

		ENSURE_OP(dimension, ==, 1);

		typedef std::vector<KeyValue<Real, integer> > SearchSet;
		typedef typename SearchSet::iterator Iterator;
		typedef typename SearchSet::const_iterator ConstIterator;
		
		SearchSet searchSet;
		searchSet.reserve(points);
		
		for (integer i = 0;i < points;++i)
		{
			searchSet.push_back(keyValue(pointSet[i][0], i));
		}

		std::sort(searchSet.begin(), searchSet.end());

		const ConstIterator begin = searchSet.begin();
		const ConstIterator end = searchSet.end();

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
				normBijection,
				nearestArray, distanceArray);
			
			if (rightIter != end)
			{
				++leftIter;
				++rightIter;
			}
			++iter;
		}
	}
	*/

}

#endif
