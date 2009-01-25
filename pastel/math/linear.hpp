#ifndef PASTELMATH_LINEAR_HPP
#define PASTELMATH_LINEAR_HPP

#include "pastel/math/linear.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <
		typename PointType,
		typename Real>
		PointType linear(
			const PointType& startPoint,
			const PointType& endPoint,
			const Real& time)
	{
		return startPoint +
			(endPoint - startPoint) * time;
	}

	template <int N, typename Real>
	Point<N, Real> linear(
		const Point<N, Real>& startPoint,
		const Point<N, Real>& endPoint,
		const PASTEL_NO_DEDUCTION(Real)& time)
    {
        return Point<N, Real>(
            asVector(startPoint) * (1 - time) +
            asVector(endPoint) * time);
    }

}

#endif
