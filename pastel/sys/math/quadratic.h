// Description: Quadratic equations
// Documentation: math_functions.txt

#ifndef PASTELSYS_QUADRATIC_H
#define PASTELSYS_QUADRATIC_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Solves a [quadratic equation] ax^2 + bx + c = 0.
	/*!
	aCoeff, bCoeff, cCoeff:
	The cofficients of a quadratic polynomial
	f(x) = ax^2 + bx + c, for which the roots
	are to be solved for.

	t0, t1:
	If the equation has solutions, filled with them,
	otherwise these are not modified. The solutions
	are ordered such that t0 <= t1. In case of a 
	double-root, t0 == t1.

	solutionsMustExist:
	If it is known that under real arithmetic the equation
	should have real solutions, then by toggling this
	flag the discriminant will always be clamped to zero.

	Returns:
	Whether the equation has real solutions.
	*/
	template <typename Real>
	bool quadratic(
		const NoDeduction<Real>& aCoeff,
		const NoDeduction<Real>& bCoeff,
		const NoDeduction<Real>& cCoeff,
		Real &t0, Real &t1,
		bool solutionsMustExist = false);

}

#include "pastel/sys/math/quadratic.hpp"

#endif
