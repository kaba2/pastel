#ifndef PASTELMATH_POINT_MEAN_HPP
#define PASTELMATH_POINT_MEAN_HPP

#include "pastel/math/statistic/point_mean.h"

namespace Pastel
{

	template <
		typename Real, 
		typename Point_Input, 
		typename Locator>
	Vector<Real, Locator::N> pointMean(
		Point_Input pointSet,
		const Locator& locator)
	{
		integer d = locator.n();
		ENSURE_OP(d, >=, 0);
	
		Vector<Real, Locator::N> result(ofDimension(d), 0);

		if (pointSet.empty() ||
			d == 0)
		{
			return result;
		}

		integer n = 0;
		while (!pointSet.empty())
		{
			auto&& point = pointSet.get();
			for (integer i = 0;i < d;++i)
			{
				result[i] += locator(point, i);
			}
			++n;

			pointSet.pop();
		}

		return result / n;
	}

}

#endif
