// Description: Variance of a point-set
// Documentation: statistics.txt

#ifndef PASTELMATH_POINT_VARIANCE_H
#define PASTELMATH_POINT_VARIANCE_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/vector.h"

// Implementation

#include "pastel/math/statistic/point_mean.h"

namespace Pastel
{

	//! Returns the point-set variance.
	/*!
	Time complexity:
	O(nd)
	where
	n = size(pointSet), and
	d = locator.n().
	*/
	template <
		typename PointSet,
		typename Real = PointSet_Real<PointSet>,
		typename Locator = PointSet_Locator<PointSet>,
		typename... ArgumentSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0>
	auto pointVariance(
		PointSet pointSet,
		ArgumentSet&&... argumentSet)
		-> Vector<Real, Locator::N>
	{
		bool biased = PASTEL_ARG_S(biased, true);
		Vector<Real, Locator::N> mean = PASTEL_ARG_S(mean, pointMean(pointSet));

		auto&& locator = pointSetLocator(pointSet);

		integer d = locator.n();
		ENSURE_OP(d, >=, 0);

		Vector<Real, Locator::N> result(ofDimension(d), 0);

		if (pointSet.empty() || d == 0)
		{
			return result;
		}

		integer n = 0;
		forEach(pointSet, [&](auto&& point)
		{
			for (integer i = 0; i < d;++i)
			{
				result[i] += 
					square(pointAxis(point, i) - mean[i]);
			}
			++n;

			return true;
		});

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
