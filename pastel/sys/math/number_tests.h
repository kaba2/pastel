// Description: Number tests
// Documentation: math_functions.txt

#ifndef PASTELSYS_NUMBER_TESTS_H
#define PASTELSYS_NUMBER_TESTS_H

#include "pastel/sys/integer_concept.h"

namespace Pastel
{

	//! Returns if the integer is a power of 2.
	/*!
	Preconditions:
	that >= 0

	returns:
	Whether that == 2^k, for some integer k >= 0.
	*/
	template <typename Integer>
	bool isPowerOfTwo(const Integer& that);

}

#include "pastel/sys/number_tests.hpp"

#endif
