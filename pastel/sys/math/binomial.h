// Description: Binomial coefficient
// Documentation: math_functions.txt

#ifndef PASTELSYS_BINOMIAL_H
#define PASTELSYS_BINOMIAL_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Computes the [binomial coefficient].
	/*!
	Preconditions:
	n >= 0

	binomial(i, n) = n! / ((n - i)!i!)

	If i < 0 or i > n, returns 0.
	*/
	template <typename Real>
	Real binomial(integer i, integer n);

}

#include "pastel/sys/math/binomial.hpp"

#endif
