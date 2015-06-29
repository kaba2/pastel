// Description: Mean of a point-set
// Documentation: statistics.txt

#ifndef PASTELMATH_POINT_MEAN_H
#define PASTELMATH_POINT_MEAN_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns the mean of a point set.
	/*!
	Time complexity:
	O(nd)
	where
	n = size(pointSet), and
	d = pointSetDimension(pointSet).

	returns:
	[sum_{i = 1}^n x_i] / n
	*/
	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
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
