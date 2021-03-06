// Description: Greatest common divisor
// Documentation: integers.txt

#ifndef PASTELSYS_GCD_H
#define PASTELSYS_GCD_H

#include "pastel/sys/integer/integer_concept.h"

namespace Pastel
{

	//! Greatest common divisor of a and b.
	/*!
	This function also returns the Bezout coefficients x and y
	such that 
	x a + y b = gcd(a, b)
	*/
	template <typename Integer>
	Integer extendedGcd(
		Integer a, Integer b,
		Integer& x, Integer& y);

	//! Greatest common divisor of abs(left) and abs(right).
	/*!
	Time complexity: (log2(left) + log2(right))^2
	*/
	template <typename Integer>
	Integer gcd(const Integer& left, const Integer& right);

}

#include "pastel/sys/integer/gcd.hpp"

#endif
