// Description: Kochanek-Bartels spline
// Documentation: interpolation.txt

#ifndef PASTELMATH_KBSPLINE_H
#define PASTELMATH_KBSPLINE_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/hermite.h"

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
		const Real& time)
	{
		Real mTension(1 - tension);
		Real mBias(1 - bias);
		Real pBias(1 + bias);
		Real mContinuity(1 - continuity);
		Real pContinuity(1 + continuity);

		Real baFactor1 =
			mTension * pBias *
			mContinuity * Real(0.5);
		Real cbFactor1 =
			mTension * mBias *
			pContinuity * Real(0.5);
		Real baFactor2 =
			mTension * pBias *
			pContinuity * Real(0.5);
		Real cbFactor2 =
			mTension * mBias *
			mContinuity * Real(0.5);

		return hermite(
			startPoint,
			(startPoint - previousPoint) * baFactor1 +
			(endPoint - startPoint) * cbFactor1,
			endPoint,
			(startPoint - previousPoint) * baFactor2 +
			(endPoint - startPoint) * cbFactor2,
			time);
	}

}

#endif
