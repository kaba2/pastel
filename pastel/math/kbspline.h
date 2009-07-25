// Description: Kochanek-Bartels spline
// Documentation: interpolation.txt

#ifndef PASTEL_KBSPLINE_H
#define PASTEL_KBSPLINE_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Evaluates a Kochanek-Bartels spline.

	/*!
	A Kochanek-Bartels spline can be used to
	interpolate a sequence of points
	p = (p1, p2, ...) excluding the first point.
	The curve can modified by choosing different
	parameters for 'tension', 'bias' and
	'continuity'. Sometimes these curves are
	also called TCB-splines. A segment of
	this curve is a special case of a cubic
	Hermite curve, see cubicHermite().
	*/

	template <
		typename PointType,
		typename Real>
		PointType kbSpline(
		const PointType& previousPoint,
		const PointType& startPoint,
		const PointType& endPoint,
		const Real& tension,
		const Real& bias,
		const Real& continuity,
		const Real& time);

}

#include "pastel/math/kbspline.hpp"

#endif
