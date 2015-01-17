// Description: Uniformly-distributed random integers

#ifndef PASTELSYS_RANDOM_INTEGER_H
#define PASTELSYS_RANDOM_INTEGER_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a uniformly distributed random unsigned integer.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	uinteger randomUinteger();

	//! Returns a uniformly distributed random unsigned integer in [0, n).
	/*!
	Time complexity: 
	O(1), if isPowerOfTwo(n) or n == 0,
	O(1) expected, otherwise.

	Exception safety: nothrow

	The n = 0 is interpreted as spanning the whole range [0, 2^w),
	where w = sizeInBits<uinteger>(). The excepted number of draws 
	made from the underlying [0, 2^w)-random-number generator is always 
	less than 2.
	*/
	uinteger randomUinteger(uinteger n);

	//! Returns a uniformly distributed random unsigned integer in [0, 2^bits).
	/*!
	Preconditions:
	0 < bits <= sizeInBits<uinteger>();
	*/
	uinteger randomUintegerBits(uinteger bits);

	//! Returns a uniformly distributed non-negative random integer in [0, n).
	/*!
	Preconditions:
	n >= 0

	The n = 0 is interpreted as spanning the whole range [0, 2^{w - 1}),
	where w = sizeInBits<integer>().
	*/
	integer randomInteger(integer n);

}

#include "pastel/sys/random/random_integer.hpp"

#endif
