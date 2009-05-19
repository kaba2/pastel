#ifndef PASTEL_KBSPLINE_HPP
#define PASTEL_KBSPLINE_HPP

#include "pastel/math/kbspline.h"

#include "pastel/sys/mytypes.h"
#include "pastel/math/hermite.h"

namespace Pastel
{

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
		const Real mTension(1 - tension);
		const Real mBias(1 - bias);
		const Real pBias(1 + bias);
		const Real mContinuity(1 - continuity);
		const Real pContinuity(1 + continuity);

		const Real baFactor1 =
			mTension * pBias *
			mContinuity * Real(0.5);
		const Real cbFactor1 =
			mTension * mBias *
			pContinuity * Real(0.5);
		const Real baFactor2 =
			mTension * pBias *
			pContinuity * Real(0.5);
		const Real cbFactor2 =
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
