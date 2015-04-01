// Description: Mean of a point-set
// Documentation: statistics.txt

#ifndef PASTELMATH_POINT_MEAN_H
#define PASTELMATH_POINT_MEAN_H

#include "pastel/sys/input/input_concept.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/real/real_concept.h"
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
	template <typename PointSet>
	auto pointMean(PointSet pointSet)
		-> Vector<PointSet_Real<PointSet>, Locator_N<PointSet_Locator<PointSet>>::value>;

}

#include "pastel/math/statistic/point_mean.hpp"

#endif
