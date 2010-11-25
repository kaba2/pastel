// Description: Quadratic equations
// Documentation: math_functions.txt

#ifndef PASTEL_QUADRATIC_H
#define PASTEL_QUADRATIC_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Solves a [quadratic equation] ax^2 + bx + c = 0.
	/*!
	If the equation has no solution, returns false and
	't0' and 't1' are left unmodified. Else returns
	true and 't0' and 't1' are assigned the roots
	such that t0 <= t1. If there is a double root,
	t0 == t1.
	*/
	template <typename Real>
	bool quadratic(
		const PASTEL_NO_DEDUCTION(Real)& aCoeff,
		const PASTEL_NO_DEDUCTION(Real)& bCoeff,
		const PASTEL_NO_DEDUCTION(Real)& cCoeff,
		Real &t0, Real &t1,
		bool solutionsMustExist = false);

}

#include "pastel/sys/quadratic.hpp"

#endif
