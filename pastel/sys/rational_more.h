// Description: More Algorithms for rational numbers
// Detail: infinity, nan, fraction, remainder, floor, toFloat, etc.

#ifndef PASTEL_RATIONAL_MORE_H
#define PASTEL_RATIONAL_MORE_H

#include "pastel/sys/rational.h"

namespace Pastel
{

	//! Computes the multiplicative inverse of 'x'.
	template <typename Integer>
	Rational<Integer> inverse(const Rational<Integer>& x);

	//! Computes the greatest integer less than or equal to x.
	template <typename Integer>
	Integer floor(const Rational<Integer>& x);

	//! Computes the smallest integer greater than or equal to x.
	template <typename Integer>
	Integer ceil(const Rational<Integer>& x);

	//! Returns (1/0).
	template <typename Integer>
	Rational<Integer> infinity(Rational<Integer>*);

	//! Returns (0/0).
	template <typename Integer>
	Rational<Integer> nan(Rational<Integer>*);

	//! Returns that == 0.
	template <typename Integer>
	bool zero(const Rational<Integer>& that);

	//! Returns that < 0.
	template <typename Integer>
	bool negative(
		const Rational<Integer>& that);

	//! Returns that > 0.
	template <typename Integer>
	bool positive(const Rational<Integer>& that);

	template <typename Real, typename Integer>
	Real toReal(const Rational<Integer>& that);

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
