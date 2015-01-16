// Description: Factorial
// Documentation: math_functions.txt

#ifndef PASTELSYS_FACTORIAL_H
#define PASTELSYS_FACTORIAL_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns the factorial.
	/*!
	Preconditions:
	i >= 0

	A table look-up is used for i in [0, 20].
	The gamma() function is used for larger i.
	Note that 20! < 2^64 - 1 < 21!.
	*/
	template <typename Real>
	Real factorial(integer i);

	//! Returns the logarithm of the factorial.
	/*!
	Preconditions:
	i >= 0

	A table look-up is used for i in [0, 256].
	The gamma() function is used for larger i.
	*/
	template <typename Real>
	Real lnFactorial(integer i);

}

#include "pastel/sys/factorial.hpp"

#endif
