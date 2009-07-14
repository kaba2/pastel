// Description: Tools to compute statistics from samples
// Detail: mean, axisAlignedVariance, etc.
// Documentation: pastelmath.txt

#ifndef PASTELMATH_STATISTICS_H
#define PASTELMATH_STATISTICS_H

#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	//! Returns the mean of a point set.

	template <int N, typename Real>
	TemporaryPoint<N, Real> mean(
		const std::vector<Point<N, Real> >& pointSet);

	//! Returns the variance along each axis.

	template <int N, typename Real>
	TemporaryVector<N, Real> axisAlignedVariance(
		const std::vector<Point<N, Real> >& pointSet,
		const Point<N, Real>& mean);

}

#include "pastel/math/statistics.hpp"

#endif
