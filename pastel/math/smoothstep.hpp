#ifndef PASTEL_SMOOTHSTEP_HPP
#define PASTEL_SMOOTHSTEP_HPP

#include "pastel/math/smoothstep.h"
#include "pastel/math/linear.h"

namespace Pastel
{

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
