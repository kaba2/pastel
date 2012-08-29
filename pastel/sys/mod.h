// Description: Remainder
// Documentation: math_functions.txt

#ifndef PASTELSYS_MOD_H
#define PASTELSYS_MOD_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Computes the [remainder].
	/*!
	Preconditions:
	n > 0
	Postconditions:
	0 <= x < n

	Returns:
	x mod n

	mod(-1, n) = n - 1, and so on
	*/
	integer mod(integer x, integer n);

	//! Returns x mod 1.
	/*!
	Postconditions:
	0 <= x < 1

	Returns:
	x - floor(x)
	*/
	real mod(real x);

	//! Returns x mod n.
	/*!
	Preconditions:
	n > 0
	Postconditions:
	0 <= x < n

	This is a convenience function that calls
	mod(x / n) * n
	*/
	real mod(real x, real n);

}

#include "pastel/sys/mod.hpp"

#endif
