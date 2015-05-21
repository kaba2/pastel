#ifndef PASTELMATH_POINT_MEAN_HPP
#define PASTELMATH_POINT_MEAN_HPP

#include "pastel/math/statistic/point_mean.h"

namespace Pastel
{

	template <
		typename PointSet,
		Requires<Models<PointSet, PointSet_Concept>>
	>
	auto pointMean(PointSet pointSet)
		-> Vector<PointSet_Real<PointSet>, Locator_N<PointSet_Locator<PointSet>>::value>
	{
		using Locator = PointSet_Locator<PointSet>;
		using Real = PointSet_Real<PointSet>;

		integer d = pointSetDimension(pointSet);
		ENSURE_OP(d, >=, 0);
	
		Vector<Real, Locator_N<Locator>::value> result(ofDimension(d), 0);

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
				result[i] += pointAxis(point, i);
			}
			++n;

			pointSetPop(pointSet);
		}

		return result / n;
	}

}

#endif
