#ifndef PASTELMATH_STATISTICS_HPP
#define PASTELMATH_STATISTICS_HPP

#include "pastel/math/statistics.h"

namespace Pastel
{

	template <int N, typename Real>
	TemporaryPoint<N, Real> mean(
		const std::vector<Point<N, Real> >& pointSet)
	{
		const integer points = pointSet.size();

		if (points == 0)
		{
			Point<N, Real> result(ofDimension(0));
			return result.asTemporary();
		}

		const integer dimension = pointSet.front().dimension();

		Point<N, Real> result(ofDimension(dimension), 0);
		for (integer i = 0;i < points;++i)
		{
			result += asVector(pointSet[i]);
		}

		asVector(result) /= points;
		
		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> axisAlignedVariance(
		const std::vector<Point<N, Real> >& pointSet,
		const Point<N, Real>& mean)
	{
		const integer points = pointSet.size();
		const integer dimension = pointSet.front().dimension();

		Vector<N, Real> result(ofDimension(dimension));
		for (integer i = 0;i < points;++i)
		{
			result += squarev(pointSet[i] - mean);
		}
		
		return result.asTemporary();
	}

}

#endif
