// Description: Statistics from samples
// Detail: mean, axisAlignedVariance, etc.
// Documentation: common.txt

#ifndef PASTELMATH_STATISTICS_H
#define PASTELMATH_STATISTICS_H

#include "pastel/sys/vector.h"

#include <vector>

namespace Pastel
{

	//! Returns the mean of a point set.

	template <int N, typename Real>
	Vector<Real, N> mean(
		const std::vector<Vector<Real, N> >& pointSet);

	//! Returns the variance along each axis.

	template <int N, typename Real>
	Vector<Real, N> axisAlignedVariance(
		const std::vector<Vector<Real, N> >& pointSet,
		const Vector<Real, N>& mean);

}

#include "pastel/math/statistics.hpp"

#endif
