// Description: Smoothstep functions
// Detail: Cubic, quartic, and quintic smoothstep functions
// Documentation: interpolation.txt

#ifndef PASTELMATH_SMOOTHSTEP_H
#define PASTELMATH_SMOOTHSTEP_H

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
		const Real& time);

	template <typename PointType, typename Real>
	PointType cubicSmoothStep(
		const PointType& from,
		const PointType& to,
		const Real& time);

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
		const Real& xSquared);

	template <typename PointType, typename Real>
	PointType quarticSmoothStep(
		const PointType& from,
		const PointType& to,
		const Real& time);

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
		const Real& time);

	template <typename PointType, typename Real>
	PointType quinticSmoothStep(
		const PointType& from,
		const PointType& to,
		const Real& time);

}

#include "pastel/math/smoothstep.hpp"

#endif
