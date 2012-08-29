#ifndef PASTELMATH_CRSPLINE_HPP
#define PASTELMATH_CRSPLINE_HPP

#include "pastel/math/crspline.h"
#include "pastel/math/hermite.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename PointType,
		typename Real>
		PointType crSpline(
		const PointType& previousPoint,
		const PointType& startPoint,
		const PointType& endPoint,
		const PointType& nextPoint,
		const Real& time)
	{
		return cubicHermite(
			startPoint,
			(endPoint - previousPoint) * 0.5,
			endPoint,
			(nextPoint - startPoint) * 0.5,
			time);
	}

}

#endif
