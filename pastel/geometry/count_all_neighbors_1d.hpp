#ifndef PASTELGEOMETRY_COUNT_ALL_NEIGHBORS_1D_HPP
#define PASTELGEOMETRY_COUNT_ALL_NEIGHBORS_1D_HPP

#include "pastel/geometry/count_all_neighbors_1d.h"

#include "pastel/sys/keyvalue.h"
#include "pastel/sys/ensure.h"

#include <tbb/parallel_for.h>

#include <algorithm>

namespace Pastel
{

	template <typename Real, int N, typename NormBijection,
	typename ConstIndexIterator, typename ConstDistanceIterator,
	typename CountIterator>
	void countAllNeighbors1d(
		const std::vector<Vector<Real, N> >& pointSet,
		const ConstIndexIterator& indexBegin,
		const ConstIndexIterator& indexEnd,
		const ConstDistanceIterator& maxDistanceBegin,
		const NormBijection& normBijection,
		const CountIterator& neighborsBegin)
	{
		PASTEL_STATIC_ASSERT(N == 1 || N == Dynamic);
		
		const integer points = pointSet.size();
		const integer indices = indexEnd - indexBegin;

		if (points == 0 || indices == 0)
		{
			return;
		}

		ENSURE1(pointSet.front().n() == 1,
			pointSet.front().n());

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

		using IndexRange = tbb::blocked_range<integer>;

		auto countNeighbors = [&](const IndexRange& range)
		{
			for (integer i = range.begin();i < range.end();++i)
			{
				PENSURE_OP(maxDistanceBegin[i], >=, 0);

				const integer index = indexBegin[i];
				const Real position = pointSet[index][0];
				const Real radius = normBijection.toNorm(maxDistanceBegin[i]);
				
				const ConstIterator leftIter = std::lower_bound(
					begin, end, 
					keyValue(position - radius, index));
				const ConstIterator rightIter = std::upper_bound(
					begin, end,
					keyValue(position + radius, index));

				neighborsBegin[i] = rightIter - leftIter;
			}
		};

		tbb::parallel_for(IndexRange(0, indices), 
			countNeighbors);
	}

}

#endif
