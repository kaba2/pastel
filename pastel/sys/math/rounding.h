// Description: Rounding
// Documentation: math_functions.txt

#ifndef PASTELSYS_ROUNDING_H
#define PASTELSYS_ROUNDING_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/integer/integer_concept.h"

#include <type_traits>

namespace Pastel
{

	//! Rounds up 'that' to the next multiple of 'to'.
	/*!
	Preconditions:
	!negative(to)

	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <Integer_Concept Integer>
	Integer roundUpTo(
		const Integer& that, const Integer& to);

	//! Rounds up to the next odd number.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <Integer_Concept Integer>
	Integer roundUpToOdd(const Integer& that);
	
	//! Rounds up to the next odd number.
	/*!
	Preconditions:
	Real is a native floating point type.

	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <std::floating_point Real>
	integer roundUpToOdd(const Real& that);

	//! Rounds up to the next even number.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <Integer_Concept Integer>
	Integer roundUpToEven(const Integer& that);

	//! Rounds up to the next even number.
	/*!
	Preconditions:
	Real is a native floating point type.

	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <std::floating_point Real>
	integer roundUpToEven(const Real& that);

	//! Rounds up to the next power of 2 (unsigned integers).
	/*!
	Preconditions:
	Integer is an unsigned native integer.

	Time complexity: O(log(SizeInBits<Integer>::value) + 1)
	Exception safety: nothrow
	*/
	template <std::unsigned_integral Integer>
	Integer roundUpToPowerOfTwo(const Integer& that);

	//! Rounds up to the next power of 2 (signed integers).
	/*!
	Preconditions:
	Integer is a signed native integer.
	The next power of 2 can be represented.

	Time complexity: O(log(SizeInBits<Integer>::value) + 1)
	Exception safety: nothrow
	*/
	template <std::signed_integral Integer>
	Integer roundUpToPowerOfTwo(const Integer& that);

	//! Rounds up 'that' to the next multiple of power of 2.
	/*!
	Preconditions:
	!negative(power)

	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <Integer_Concept Integer>
	Integer roundUpToPowerOfTwo(
		const Integer& that, 
		integer power);

}

#include "pastel/sys/math/rounding.hpp"

#endif
