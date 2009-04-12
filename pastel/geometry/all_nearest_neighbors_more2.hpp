#ifndef PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_MORE2_HPP
#define PASTELGEOMETRY_ALL_NEAREST_NEIGHBORS_MORE2_HPP

#include "pastel/geometry/all_nearest_neighbors.h"

#include "pastel/sys/unorderedmap.h"

#include <vector>

namespace Pastel
{

	namespace Detail_AllNearestNeighborsOwn
	{

		template <int N, typename Real>
		class AllNearestNeighborsOwn
		{
		public:
			AllNearestNeighborsOwn(
				const std::vector<Point<N, Real> >& pointSet,
				integer kNearest,
				Array<2, integer>& nearestSet)
				: pointSet_(pointSet)
				, kNearest_(kNearest)
				, nearestSet_(nearestSet)
			{
			}

			void work();

		private:
			class AxisCompare
			{
			public:
				AxisCompare(
					integer dimension,
					const std::vector<Point<N, Real> >& pointSet)
					: dimension_(dimension)
					, pointSet_(pointSet)
				{
				}

				bool operator()(integer leftIndex, integer rightIndex) const
				{
					const Real left = pointSet_[leftIndex][dimension_];
					const Real right = pointSet_[rightIndex][dimension_];
					
					if (left < right)
					{
						return true;
					}
					if (right < left)
					{
						return false;
					}

					return leftIndex < rightIndex;
				}

			private:
				integer dimension_;
				const std::vector<Point<N, Real> >& pointSet_;
			};

			class Entry
			{
			public:
				Entry(const Real& distance2,
					integer index,
					integer dimension)
					: distance2_(distance2)
					, index_(index)
					, dimension_(dimension)
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

					if (index_ < that.index_)
					{
						return true;
					}
					if (that.index_ < index_)
					{
						return false;
					}

					return dimension_ < that.dimension_;
				}

				Real distance2_;
				integer index_;
				integer dimension_;
			};

			typedef std::vector<integer> IndexSet;
			typedef IndexSet::iterator IndexIterator;

			typedef std::vector<Integer2> RangeList;
			typedef RangeList::iterator RangeIterator;

			typedef UnorderedMap<integer, integer> HitMap;
			typedef HitMap::iterator HitIterator;

			void updateAxisNearestSet(
				integer dimension,
				const Real& pivot,
				std::set<Entry>& axisNearestSet,
				RangeList& rangeList,
				const IndexSet& indexSet);

			const std::vector<Point<N, Real> >& pointSet_;
			integer kNearest_;
			Array<2, integer>& nearestSet_;

			integer searchSum_;
		};

		template <int N, typename Real>
		void AllNearestNeighborsOwn<N, Real>::work()
		{
			const integer points = pointSet_.size();

			searchSum_ = 0;

			std::vector<IndexSet> indexSetList(N);
			RangeList rangeList(N);

			// Sort the point projections
			// on each axis.

			for (integer i = 0;i < points;++i)
			{
				indexSetList.front().push_back(i);
			}

			for (integer j = 1;j < N;++j)
			{
				indexSetList[j] = indexSetList.front();
			}

			for (integer j = 0;j < N;++j)
			{
				std::sort(
					indexSetList[j].begin(),
					indexSetList[j].end(),
					AxisCompare(j, pointSet_));
			}

			log() << "Done sorting." << logNewLine;

			for (integer i = 0;i < points;++i)
			{
				HitMap hitMap;
				std::set<Entry> axisNearestSet;

				const Point<N, Real>& pivot = pointSet_[i];

				for (integer j = 0;j < N;++j)
				{
					const IndexIterator iter = 
						std::lower_bound(indexSetList[j].begin(), indexSetList[j].end(),
						i, AxisCompare(j, pointSet_));
					ASSERT(iter != indexSetList[j].end());
					ASSERT(*iter == i);

					rangeList[j] = Integer2(&*iter - &*indexSetList[j].begin());

					updateAxisNearestSet(j, pivot[j], axisNearestSet, rangeList, indexSetList[j]);
				}

				ASSERT(axisNearestSet.size() == N);

				integer nearestFound = 0;
				while(!axisNearestSet.empty())
				{
					const Entry entry = *axisNearestSet.begin();
					axisNearestSet.erase(axisNearestSet.begin());
					++searchSum_;

					const HitIterator hitIter = 
						hitMap.insert(std::make_pair(entry.index_, 0)).first;

					integer& hitCount = hitIter->second;
					ASSERT2(hitCount >= 0 && hitCount < N, hitCount, N);
					++hitCount;

					if (hitCount == N)
					{
						nearestSet_(i, nearestFound) = entry.index_;
						++nearestFound;
						if (nearestFound == kNearest_)
						{
							break;
						}
					}

					updateAxisNearestSet(entry.dimension_, pivot[entry.dimension_],
						axisNearestSet, rangeList, indexSetList[entry.dimension_]);
				}

				ASSERT2(nearestFound == kNearest_, nearestFound, kNearest_);
			}

			log() << "Search average = " << (real)searchSum_ / (points * N) << logNewLine;
		}	

		template <int N, typename Real>
		void AllNearestNeighborsOwn<N, Real>::updateAxisNearestSet(
			integer dimension,
			const Real& pivot,
			std::set<Entry>& axisNearestSet,
			RangeList& rangeList,
			const IndexSet& indexSet)
		{
			ASSERT(rangeList[dimension][0] >= 0);
			ASSERT(rangeList[dimension][1] < pointSet_.size());

			const integer leftIndex = rangeList[dimension][0] - 1;
			const integer rightIndex = rangeList[dimension][1] + 1;

			const bool leftValid = (leftIndex >= 0);
			const bool rightValid = (rightIndex < pointSet_.size());

			ASSERT(leftValid || rightValid);

			if (leftValid && !rightValid)
			{
				const integer leftPointIndex = indexSet[leftIndex];
				const Real leftDistance2 = square(
					pointSet_[leftPointIndex][dimension] - pivot);

				axisNearestSet.insert(
					Entry(leftDistance2, leftPointIndex, dimension));
				rangeList[dimension][0] = leftIndex;
			}
			else if (!leftValid && rightValid)
			{
				const integer rightPointIndex = indexSet[rightIndex];
				const Real rightDistance2 = square(
					pointSet_[rightPointIndex][dimension] - pivot);

				axisNearestSet.insert(
					Entry(rightDistance2, rightPointIndex, dimension));
				rangeList[dimension][1] = rightIndex;
			}
			else if (leftValid && rightValid)
			{
				const integer leftPointIndex = indexSet[leftIndex];
				const Real leftDistance2 = square(
					pointSet_[leftPointIndex][dimension] - pivot);

				const integer rightPointIndex = indexSet[rightIndex];
				const Real rightDistance2 = square(
					pointSet_[rightPointIndex][dimension] - pivot);

				if (leftDistance2 < rightDistance2)
				{
					axisNearestSet.insert(
						Entry(leftDistance2, leftPointIndex, dimension));
					rangeList[dimension][0] = leftIndex;
				}
				else
				{
					axisNearestSet.insert(
						Entry(rightDistance2, rightPointIndex, dimension));
					rangeList[dimension][1] = rightIndex;
				}
			}
		}

	}

	template <int N, typename Real>
	void allNearestNeighborsOwn(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		Array<2, integer>& nearestSet)
	{
		Detail_AllNearestNeighborsOwn::AllNearestNeighborsOwn<N, Real>
			computation(pointSet, kNearest, nearestSet);

		computation.work();
	}

}

#endif
