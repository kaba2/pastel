// Description: Number tests
// Documentation: math_functions.txt

#ifndef PASTELSYS_NUMBER_TESTS_H
#define PASTELSYS_NUMBER_TESTS_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns if the integer is [odd].
	template <typename Integer>
	bool odd(const Integer& x);

	//! Returns if the integer is [even].
	template <typename Integer>
	bool even(const Integer& x);

	//! Returns if the integer is a power of 2.
	/*!
	Preconditions:
	that >= 0

	returns:
	Whether that == 2^k, for some integer k >= 0.
	*/
	bool isPowerOfTwo(integer that);

}

#include "pastel/sys/number_tests.hpp"

#endif
