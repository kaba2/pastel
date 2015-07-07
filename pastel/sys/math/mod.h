// Description: Remainder
// Documentation: math_functions.txt

#ifndef PASTELSYS_MOD_H
#define PASTELSYS_MOD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/integer/integer_concept.h"

#include <type_traits>

namespace Pastel
{

	//! Returns x mod 2^n for signed integers.
	/*!
	Preconditions:
	n >= 0

	Here's what mod 2^2 computes:
	-6 -5 -4 -3 -2 -1 0 1 2 3 4 5 6
	 2  3  0  1  2  3 0 1 2 3 0 1 2
	*/
	template <typename Integer, Requires<std::is_signed<Integer>> = 0>
	Integer modPowerOfTwo(const Integer& x, integer n);

	//! Returns x mod 2^n for unsigned integers.
	/*!
	Preconditions:
	n >= 0

	See the documentation for the signed version.
	*/
	template <typename Integer, Requires<std::is_unsigned<Integer>> = 0>
	Integer modPowerOfTwo(const Integer& x, integer n);

	//! Returns x mod n.
	/*!
	Preconditions:
	n > 0

	Here's what mod 3 computes:
	-6 -5 -4 -3 -2 -1 0 1 2 3 4 5 6
	 0  1  2  0  1  2 0 1 2 0 1 2 0
	*/
	template <typename Integer>
	Integer mod(const Integer& x, const Integer& n);

	//! Returns x mod 1.
	/*!
	Postconditions:
	0 <= x < 1

	Returns:
	x - floor(x)
	*/
	template <typename Real>
	Real realMod(const Real& x);

	//! Returns x mod n.
	/*!
	Preconditions:
	n > 0

	This is a convenience function that returns
	mod(x / n) * n
	*/
	template <typename Real>
	Real realMod(const Real& x, const Real& n);

}

#include "pastel/sys/math/mod.hpp"

#endif
