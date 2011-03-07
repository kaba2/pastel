// Description: Bernstein polynomials
// Documentation: math_functions.txt

#ifndef PASTEL_BERNSTEIN_H
#define PASTEL_BERNSTEIN_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Computes the i:th n-degree [Bernstein polynomial] at t.
	/*!
	Preconditions:
	n >= 0
	i >= 0
	i <= n

	The Bernstein polynomials are the terms
	of the binomial expansion of ((1 - t) + t)^n = 1.
	B_{i, n}(t) = binomial(i, n) t^(n - i) (1 - t)^i
	*/
	template <typename Real>
	Real bernstein(integer i, integer n, const Real& t);

}

#include "pastel/sys/bernstein.hpp"

#endif
