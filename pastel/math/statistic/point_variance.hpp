#ifndef PASTELMATH_POINT_VARIANCE_HPP
#define PASTELMATH_POINT_VARIANCE_HPP

#include "pastel/math/statistic/point_variance.h"

namespace Pastel
{

	template <
		typename PointSet,
		typename Real,
		typename Locator>
	auto pointVariance(
		PointSet pointSet,
		bool biased,
		const Vector<Real, Locator::N>& mean)
		-> Vector<Real, Locator::N>
	{
		auto&& locator = pointSetLocator(pointSet);

		integer d = locator.n();
		ENSURE_OP(d, >=, 0);

		Vector<Real, Locator::N> result(ofDimension(d), 0);

		if (pointSetEmpty(pointSet) || d == 0)
		{
			return result;
		}

		integer n = 0;
		while(!pointSetEmpty(pointSet))
		{
			auto&& point = pointSetGet(pointSet);
			for (integer i = 0; i < d;++i)
			{
				result[i] += 
					square(locator(pointPoint(point), i) - mean[i]);
			}
			++n;

			pointSetPop(pointSet);
		}

		if (n == 0)
		{
			return result;
		}

		if (biased)
		{
			return result / n;
		}
		else
		{
			if (n > 1)
			{
				return result / (n - 1);
			}
			else
			{
				result.set(0);
			}
		}
		
		return result;
	}


}

#endif
