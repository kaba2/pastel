#ifndef PASTEL_INTERVAL_TOOLS_HPP
#define PASTEL_INTERVAL_TOOLS_HPP

#include "pastel/math/interval_tools.h"

#include <algorithm>

namespace Pastel
{

	template <typename Real>
	Interval<Real> intersect(
		const Interval<Real>& aInterval,
		const Interval<Real>& bInterval)
	{
		if (aInterval.max() < bInterval.min() ||
			bInterval.max() < aInterval.max())
		{
			return Interval<Real>();
		}

		return Interval<Real>(
			std::max(aInterval.min(), bInterval.min()),
			std::min(aInterval.max(), bInterval.max()));
	}

	template <typename Real>
	Interval<Real> boundingInterval(
		const Interval<Real>& aInterval,
		const Interval<Real>& bInterval)
	{
		return Interval<Real>(
			std::min(aInterval.min(), bInterval.min()),
			std::max(aInterval.max(), bInterval.max()));
	}

}

#endif
