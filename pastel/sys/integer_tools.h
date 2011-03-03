// Description: Integer algorithms
// Detail: gcd, extendedGcd, etc.
// Documentation: integers.txt

#ifndef PASTEL_INTEGER_TOOLS_H
#define PASTEL_INTEGER_TOOLS_H

namespace Pastel
{

	//! Returns the greatest common divisor of a and b.
	/*!
	This function also returns the Bezout coefficients x and y
	such that 
	x a + y b = gcd(a, b)
	*/

	integer extendedGcd(
		integer a, integer b,
		integer& x, integer& y);

	//! Returns the greatest common divisor of 'mabs(left)' and 'mabs(right)'.
	/*!
	Time complexity: (log2(left) + log2(right))^2
	*/

	template <typename Integer>
	Integer gcd(const Integer& left, const Integer& right);

}

#include "pastel/sys/integer_tools.hpp"

#endif
