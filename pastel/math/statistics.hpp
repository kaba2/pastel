#ifndef PASTELMATH_STATISTICS_HPP
#define PASTELMATH_STATISTICS_HPP

#include "pastel/math/statistics.h"

namespace Pastel
{

	template <int N, typename Real>
	TemporaryPoint<Real, N> mean(
		const std::vector<Point<Real, N> >& pointSet)
	{
		const integer points = pointSet.size();

		if (points == 0)
		{
			Point<Real, N> result(ofDimension(0));
			return result.asTemporary();
		}

		const integer dimension = pointSet.front().dimension();

		Point<Real, N> result(ofDimension(dimension), 0);
		for (integer i = 0;i < points;++i)
		{
			result += asVector(pointSet[i]);
		}

		asVector(result) /= points;
		
		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryVector<Real, N> axisAlignedVariance(
		const std::vector<Point<Real, N> >& pointSet,
		const Point<Real, N>& mean)
	{
		const integer points = pointSet.size();
		const integer dimension = pointSet.front().dimension();

		Vector<Real, N> result(ofDimension(dimension));
		for (integer i = 0;i < points;++i)
		{
			result += squarev(pointSet[i] - mean);
		}
		
		return result.asTemporary();
	}

}

#endif
