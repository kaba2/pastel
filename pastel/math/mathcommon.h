/*!
\file
\brief Various functions for common mathematical needs.
*/

#ifndef PASTELMATH_MATHCOMMON_H
#define PASTELMATH_MATHCOMMON_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/mathlibrary.h"

#include <vector>

namespace Pastel
{

	//! Computes the binomial coefficient choose(n, i) = n! / ((n - i)!i!).
	/*!
	Preconditions:
	n >= 0

	If i < 0 or i > n, returns 0.
	*/

	template <typename Real>
	Real choose(integer n, integer i);

	//! Computes the factorial i!.
	/*!
	Preconditions:
	i >= 0

	0! = 1
	i! = i * (i - 1)!
	*/

	template <typename Real>
	Real factorial(integer i);

	//! Computes the i:th n-degree Bernstein polynomial at t.
	/*!
	Preconditions:
	n >= 0
	0 <= i <= n

	The Bernstein polynomials are the terms
	of the binomial expansion of 1 = 1^n = ((1 - t) + t)^n.
	*/

	template <typename Real>
	Real bernstein(integer n, integer i, const Real& t);

	//! Solves a quadratic equation ax^2 + bx + c = 0.

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

#include "pastel/math/mathcommon.hpp"

#endif
