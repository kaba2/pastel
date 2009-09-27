#ifndef PASTELMATH_STATISTICS_HPP
#define PASTELMATH_STATISTICS_HPP

#include "pastel/math/statistics.h"

namespace Pastel
{

	template <typename Real, int N>
	Vector<Real, N> mean(
		const std::vector<Vector<Real, N> >& pointSet)
	{
		const integer points = pointSet.size();

		if (points == 0)
		{
			Vector<Real, N> result(ofDimension(0));
			return result;
		}

		const integer dimension = pointSet.front().dimension();

		Vector<Real, N> result(ofDimension(dimension), 0);
		for (integer i = 0;i < points;++i)
		{
			result += pointSet[i];
		}

		result /= points;
		
		return result;
	}

	template <typename Real, int N>
	Vector<Real, N> axisAlignedVariance(
		const std::vector<Vector<Real, N> >& pointSet,
		const Vector<Real, N>& mean)
	{
		const integer points = pointSet.size();
		const integer dimension = pointSet.front().dimension();

		Vector<Real, N> result(ofDimension(dimension));
		for (integer i = 0;i < points;++i)
		{
			result += squarev(pointSet[i] - mean);
		}
		
		return result;
	}

}

#endif
