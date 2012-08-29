// Description: Cubic and quintic Hermite polynomials
// Documentation: interpolation.txt

#ifndef PASTELMATH_HERMITE_H
#define PASTELMATH_HERMITE_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Evaluates a cubic Hermite curve.

	/*!
	A cubic Hermite curve f is a cubic polynomial
	that satisfies the following constraints:
	f(0) = startPoint
	f(1) = endPoint
	f'(0) = startVelocity
	f'(1) = endVelocity

	This can be used to interpolate two points
	and their velocity vectors.

	If startVelocity = endVelocity = 0,
	then you should consider the 'smoothStep()' functions
	in 'pastel/math/smoothstep.h' instead.
	*/

	template <
		typename PointType,
		typename VectorType,
		typename Real>
		PointType cubicHermite(
		const PointType& startPoint,
		const VectorType& startVelocity,
		const PointType& endPoint,
		const VectorType& endVelocity,
		const Real& time);

	//! Evaluates a quintic Hermite curve.

	/*!
	A quintic Hermite curve f is a quintic polynomial
	that satisfies the following constraints:
	f(0) = startPoint
	f(1) = endPoint
	f'(0) = startVelocity
	f'(1) = endVelocity
	f'(0) = startAcceleration
	f'(1) = endAcceleration

	This can be used to interpolate two points,
	their velocity vectors and their acceleration
	vectors.

	If startVelocity = endVelocity = 0 and
	startAcceleration = endAcceleration = 0
	then you should consider the 'smoothStep()' functions
	in 'pastel/math/smoothstep.h' instead.
	*/

	template <
		typename PointType,
		typename VectorType,
		typename Real>
		PointType quinticHermite(
		const PointType& startPoint,
		const VectorType& startVelocity,
		const VectorType& startAcceleration,
		const PointType& endPoint,
		const VectorType& endVelocity,
		const VectorType& endAcceleration,
		const Real& time);

}

#include "pastel/math/hermite.hpp"

#endif
