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
	in 'pastel/math/interpolation/smoothstep.h' instead.
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
			const Real& x)
	{
		// Requirements:
		// h(0) = startPoint
		// h(1) = endPoint
		// h'(0) = startVelocity
		// h'(1) = endVelocity

		// Solution:
		// h1(x) = -2 * x^3 + 3 * x^2
		// h2(x) = x^3 - 2 * x^2 + x
		// h3(x) = x^3 - x^2
		// h(x) = startPoint +
		//        (endPoint - startPoint) * h1(x) +
		//        startVelocity * h2(x) +
		//        endVelocity * h3(x)

		const Real x2(x * x);
		const Real x3(x2 * x);

		const Real h1(-2 * x3 + 3 * x2);
		const Real h2(x3 - 2 * x2 + x);
		Real h3(x3 - x2);

		return PointType(
			startPoint +

			(endPoint - startPoint) * h1 +
			startVelocity * h2 +
			endVelocity * h3);
	}

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
	in 'pastel/math/interpolation/smoothstep.h' instead.
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
			const Real& x)
	{
		// Requirements:
		// h(0) = startPoint
		// h(1) = endPoint
		// h'(0) = startVelocity
		// h'(1) = endVelocity
		// h''(0) = startAcceleration
		// h''(1) = endAcceleration
		//
		// Solution:
		// h1(x) = 6 * x^5 - 15 * x^4 + 10 * x^3
		// h2(x) = -3 * x^5 + 8 * x^4 + x - 6 * x^3
		// h3(x) = -0.5 * x^5 + 1.5 * x^4 - 1.5 * x^3 + 0.5 * x^2
		// h4(x) = -3 * x^5 + 7 * x^4 - 4 * x^3
		// h5(x) = 0.5 * x^5 - x^4 + 0.5 * x^3
		// h(x) = startPoint +
		//        (endPoint - startPoint) * h1(x) +
		//        startVelocity * h2(x) +
		//        endVelocity * h3(x) +
		//        startAcceleration * h4(x) +
		//        endAcceleration * h5(x)

		const Real x2(x * x);
		const Real x3(x2 * x);
		const Real x4(x3 * x);
		const Real x5(x4 * x);

		const Real h1(6 * x5 - 15 * x4 + 10 * x3);
		const Real h2(-3 * x5 + 8 * x4 + x - 6 * x3);
		const Real h3(0.5 * (-x5 + x2) + 1.5 * (x4 - x3));
		const Real h4(-3 * x5 + 7 * x4 - 4 * x3);
		const Real h5(0.5 * (x5 + x3) - x4);

		return PointType(
			startPoint +
			(endPoint - startPoint) * h1 +
			startVelocity * h2 +
			endVelocity * h3 +
			startAcceleration * h4 +
			endAcceleration * h5);
	}

}

#endif
