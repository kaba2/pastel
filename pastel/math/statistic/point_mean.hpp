#ifndef PASTELMATH_POINT_MEAN_HPP
#define PASTELMATH_POINT_MEAN_HPP

#include "pastel/math/statistic/point_mean.h"

namespace Pastel
{

	template <
		typename PointSet,
		typename Real,
		typename Locator>
		auto pointMean(PointSet pointSet)
		-> Vector<Real, Locator::N>
	{
		PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

		auto&& locator = pointSetLocator(pointSet);

		integer d = locator.n();
		ENSURE_OP(d, >=, 0);
	
		Vector<Real, Locator::N> result(ofDimension(d), 0);

		if (pointSetEmpty(pointSet) ||
			d == 0)
		{
			return result;
		}

		integer n = 0;
		while (!pointSetEmpty(pointSet))
		{
			auto&& point = pointSetGet(pointSet);
			for (integer i = 0;i < d;++i)
			{
				result[i] += locator(pointPoint(point), i);
			}
			++n;

			pointSetPop(pointSet);
		}

		return result / n;
	}

}

#endif
