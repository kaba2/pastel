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

	template <int N, typename Real, typename NormBijection>
	void countAllNeighbors1d(
		const std::vector<Point<N, Real> >& pointSet,
		const std::vector<PASTEL_NO_DEDUCTION(Real)>& maxDistanceSet,
		const NormBijection& normBijection,
		std::vector<integer>& countSet)
	{
		BOOST_STATIC_ASSERT(N == 1 || N == Dynamic);
		
		const integer points = pointSet.size();
		countSet.resize(points);

		if (points == 0)
		{
			return;
		}

		ENSURE1(pointSet.front().dimension() == 1,
			pointSet.front().dimension());
		ENSURE2(maxDistanceSet.size() == pointSet.size(),
			maxDistanceSet.size(), pointSet.size());

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

#pragma omp parallel for
		for (integer i = 0;i < points;++i)
		{
			const integer index = searchSet[i].value();
			const Real radius = normBijection.toNorm(maxDistanceSet[index]);
			const Real left = searchSet[i].key() - radius;
			const Real right = searchSet[i].key() + radius;
			
			const ConstIterator leftIter = std::lower_bound(
				searchSet.begin(), searchSet.end(), keyValue(left, index));
			const ConstIterator rightIter = std::upper_bound(
				searchSet.begin(), searchSet.end(), keyValue(right, index));

			// The search point is itself in the set, so
			// we need to subtract that.
			countSet[index] = (rightIter - leftIter) - 1;
			ASSERT1(countSet[index] >= 0, countSet[index]);
		}
	}


}

#endif
