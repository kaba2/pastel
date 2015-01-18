#ifndef PASTELMATH_POINT_VARIANCE_HPP
#define PASTELMATH_POINT_VARIANCE_HPP

#include "pastel/math/statistic/point_variance.h"

namespace Pastel
{

	template <
		typename Real, 
		typename Point_Input, 
		typename Locator>
	Vector<Real, Locator::N> pointVariance(
		Point_Input pointSet,
		const Locator& locator,
		bool biased,
		const Vector<Real, Locator::N>& mean)
	{
		integer d = locator.n();
		ENSURE_OP(d, >=, 0);

		Vector<Real, Locator::N> result(ofDimension(d), 0);

		if (pointSet.empty() || d == 0)
		{
			return result;
		}

		integer n = 0;
		while(!pointSet.empty())
		{

			auto&& point = pointSet.get();
			for (integer i = 0; i < d;++i)
			{
				result[i] += 
					square(locator(point, i) - mean[i]);
			}
			++n;

			pointSet.pop();
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
