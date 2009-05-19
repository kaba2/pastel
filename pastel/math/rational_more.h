/*!
\file
\brief Free function interface of the Rational class.
*/

#ifndef PASTEL_RATIONAL_MORE_H
#define PASTEL_RATIONAL_MORE_H

#include "pastel/math/rational.h"

namespace Pastel
{

	//! Returns (1/0).

	template <typename Integer>
	Rational<Integer> infinity(Rational<Integer>*);

	//! Returns (0/0).

	template <typename Integer>
	Rational<Integer> nan(Rational<Integer>*);

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

	//! Computes the greatest integer less than or equal to x.

	template <typename Integer>
	Integer floor(const Rational<Integer>& x);

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
	Rational<Integer> inverse(const Rational<Integer>& x);

	template <typename Integer>
	bool negative(
		const Rational<Integer>& that);

	template <typename Integer>
	bool positive(const Rational<Integer>& that);

	template <typename Integer>
	bool zero(const Rational<Integer>& that);

	template <typename Integer>
	float toFloat(const Rational<Integer>& that);

	template <typename Integer>
	Rational<Integer> mabs(const Rational<Integer>& that);

}

#include "pastel/math/rational_more.hpp"

#endif
