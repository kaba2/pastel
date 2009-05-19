/*!
\file
\brief A function to perform linear interpolation between points.
*/

#ifndef PASTEL_LINEAR_H
#define PASTEL_LINEAR_H

#include "pastel/sys/mytypes.h"

#include "pastel/sys/point.h"

namespace Pastel
{

	//! Interpolates linearly between points.

	/*!
	linear(startPoint, endPoint, time) =
	startPoint + time * (endPoint - startPoint)
	*/

	template <
		typename PointType,
		typename Real>
		PointType linear(
			const PointType& startPoint,
			const PointType& endPoint,
			const Real& time);

	template <int N, typename Real>
	Point<N, Real> linear(
		const Point<N, Real>& startPoint,
		const Point<N, Real>& endPoint,
		const PASTEL_NO_DEDUCTION(Real)& time);

}

#include "pastel/math/linear.hpp"

#endif
