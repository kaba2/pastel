// Description: Algorithms for Interval's
// Detail: intersect, boundingInterval, etc.

#ifndef PASTELMATH_INTERVAL_TOOLS_H
#define PASTELMATH_INTERVAL_TOOLS_H

#include "pastel/math/interval.h"

namespace Pastel
{

	template <typename Real>
	Interval<Real> intersect(
		const Interval<Real>& aInterval,
		const Interval<Real>& bInterval);

	template <typename Real>
	Interval<Real> boundingInterval(
		const Interval<Real>& aInterval,
		const Interval<Real>& bInterval);

}

#include "pastel/math/interval_tools.hpp"

#endif
