// Description: Smoothstep functions
// Detail: Cubic, quartic, and quintic smoothstep functions
// Documentation: interpolation.txt

#ifndef PASTELMATH_SMOOTHSTEP_H
#define PASTELMATH_SMOOTHSTEP_H

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	//! Computes a smoothstep function based on a cubic polynomial.
	/*!
	This is a cubic polynomial function h that satisfies:

	h(x) = -2 * x^3 + 3 * x^2
	h(0) = 0
	h(1) = 1
	h'(x) = -6x^2 + 6x
	h'(0) = 0
	h'(1) = 0
	*/
	template <typename Real>
	Real cubicSmoothStep(
		const Real& x)
	{
		// Let
		// h(x) = ax^3 + bx^2 + cx + d
		//
		// Require:
		// h(0) = 0
		// h(1) = 1
		// h'(0) = 0
		// h'(1) = 0
		//
		// This leads to the solution
		// h(x) = -2 * x^3 + 3 * x^2

		return (x * x * (3 - 2 * x));
	}

	template <typename PointType, typename Real>
	PointType cubicSmoothStep(
		const PointType& from,
		const PointType& to,
		const Real& time)
	{
		return linear(from, to, Pastel::cubicSmoothStep(time));
	}

	//! Computes a smoothstep function based on a quartic polynomial.
	/*!
	This is a quartic polynomial function h that satisfies:

	h(x) = -x^4 + 2x^2
	h(0) = 0
	h(1) = 1
	h'(x) = -4x^3 + 4x
	h'(0) = 0
	h'(1) = 0

	The function is not symmetric: it does not hold that
	h(x) = 1 - h(1 - x)

	Why would you use this function instead of the
	'cubicSmoothStep()' function which is symmetric and
	of lower order?

	The interesting thing about this function is that it only
	uses even powers of x. Therefore, rather than taking x
	as an input, we take its square x^2 as an input.
	This is especially useful when x represents distance.
	In this case you can avoid taking the square root.
	*/
	template <typename Real>
	Real quarticSmoothStep(
		const Real& xSquared)
	{
		// Let
		// h(x) = ax^4 + bx^3 + cx^2 + dx + e
		//
		// Require:
		// h(0) = 0
		// h(1) = 1
		// h'(0) = 0
		// h'(1) = 0
		// b = 0
		// d = 0
		//
		// There are 6 requirements but only
		// 5 degrees of freedom. However,
		// d = 0 is equivalent to h'(0) = 0
		// so there are actually only 5
		// requirements.
		//
		// This leads to the solution
		// h(x) = -r^4 + 2r^2

		return -xSquared * xSquared + 2 * xSquared;
	}

	template <typename PointType, typename Real>
	PointType quarticSmoothStep(
		const PointType& from,
		const PointType& to,
		const Real& time)
	{
		return linear(from, to, Pastel::quarticSmoothStep(time));
	}

	//! Computes a smoothstep function based on a quintic polynomial.
	/*!
	This is a quintic polynomial function h that satisfies:

	h(x) = 6 * x^5 - 15 * x^4 + 10 * x^3
	h(0) = 0
	h(1) = 1
	h'(x) = 30x^4 - 60x^3 + 30x^2
	h'(0) = 0
	h'(1) = 0
	h''(x) = 120x^3 - 180x^2 + 60x
	h''(0) = 0
	h''(1) = 0
	*/
	template <typename Real>
	Real quinticSmoothStep(
		const Real& x)
	{
		// Let
		// h(x) = ax^5 + bx^4 + cx^3 + dx^2 + ex + f
		//
		// Requirements:
		// h(0) = 0
		// h(1) = 1
		// h'(0) = 0
		// h'(1) = 0
		// h''(0) = 0
		// h''(1) = 0
		//
		// This leads to the solution:
		// h(x) = 6 * x^5 - 15 * x^4 + 10 * x^3

		return (x * x * x *
			(10 + x * (-15 + x * 6)));
	}

	template <typename PointType, typename Real>
	PointType quinticSmoothStep(
		const PointType& from,
		const PointType& to,
		const Real& time)
	{
		return linear(from, to, Pastel::quinticSmoothStep(time));
	}

}

#endif
