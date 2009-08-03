#ifndef PASTEL_COUNT_ALL_NEIGHBORS_1D_HPP
#define PASTEL_COUNT_ALL_NEIGHBORS_1D_HPP

#include "pastel/geometry/count_all_neighbors_1d.h"

#include "pastel/sys/keyvalue.h"
#include "pastel/sys/pastelomp.h"
#include "pastel/sys/ensure.h"

#include <boost/static_assert.hpp>

#include <algorithm>

namespace Pastel
{

	template <int N, typename Real, typename NormBijection,
	typename ConstIndexIterator, typename ConstDistanceIterator,
	typename CountIterator>
	void countAllNeighbors1d(
		const std::vector<Point<Real, N> >& pointSet,
		const ConstIndexIterator& indexBegin,
		const ConstIndexIterator& indexEnd,
		const ConstDistanceIterator& maxDistanceBegin,
		const NormBijection& normBijection,
		const CountIterator& neighborsBegin)
	{
		BOOST_STATIC_ASSERT(N == 1 || N == Dynamic);
		
		const integer points = pointSet.size();
		const integer indices = indexEnd - indexBegin;

		if (points == 0 || indices == 0)
		{
			return;
		}

		ENSURE1(pointSet.front().dimension() == 1,
			pointSet.front().dimension());

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

#		pragma omp parallel for
		for (integer i = 0;i < indices;++i)
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
	}


}

#endif
