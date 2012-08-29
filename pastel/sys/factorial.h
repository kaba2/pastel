// Description: Factorial
// Documentation: math_functions.txt

#ifndef PASTELSYS_FACTORIAL_H
#define PASTELSYS_FACTORIAL_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Computes the [factorial].
	/*!
	Preconditions:
	i >= 0
	*/
	template <typename Real>
	Real factorial(integer i);

	//! Computes the [factorial].
	/*!
	Preconditions:
	i >= 0
	i <= 256
	*/
	PASTELSYS real64 lnFactorialReal64(integer i);

	//! Computes the [logarithm of factorial].
	/*!
	Preconditions:
	i >= 0
	i <= 256
	*/
	template <typename Real>
	Real lnFactorial(integer i);

}

#include "pastel/sys/factorial.hpp"

#endif
