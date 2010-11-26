// Description: More Algorithms for rational numbers
// Detail: infinity, nan, fraction, remainder, floor, toFloat, etc.

#ifndef PASTEL_RATIONAL_MORE_H
#define PASTEL_RATIONAL_MORE_H

#include "pastel/sys/rational.h"

namespace Pastel
{

	//! Computes the fractional part of x e [0, 1[.
	/*!
	This function is equivalent to remainder(x, 1).
	This version offers an optimization opportunity
	in this case.
	*/
	template <typename Integer>
	Rational<Integer> fraction(
		const Rational<Integer>& x);

	//! Computes the remainder of (x / divider) e [0, divider[.
	template <typename Integer>
	Rational<Integer> remainder(
		const Rational<Integer>& x,
		const Rational<Integer>& divider);

	// Optimization functions

	template <typename Integer>
	Rational<Integer> multiplyByPowerOf2(
		const Rational<Integer>& x,
		integer power);

	template <typename Integer>
	Rational<Integer> divideByPowerOf2(
		const Rational<Integer>& x,
		integer power);

	template <typename Integer>
	Rational<Integer> mabs(const Rational<Integer>& that);

}

#include "pastel/sys/rational_more.hpp"

#endif
