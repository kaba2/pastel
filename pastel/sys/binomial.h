// Description: Binomial coefficient

#ifndef PASTEL_BINOMIAL_H
#define PASTEL_BINOMIAL_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Computes the [binomial coefficient].
	/*!
	Preconditions:
	n >= 0

	choose(n, i) = n! / ((n - i)!i!)

	If i < 0 or i > n, returns 0.
	*/
	template <typename Real>
	Real choose(integer n, integer i);

}

#include "pastel/sys/binomial.hpp"

#endif
