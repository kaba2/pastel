// Description: Rounding
// Documentation: math_functions.txt

#ifndef PASTELSYS_ROUNDING_H
#define PASTELSYS_ROUNDING_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/integer_concept.h"

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
	template <typename Integer>
	Integer roundUpTo(
		const Integer& that, const Integer& to);

	//! Rounds up to the next odd number.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Integer>
	Integer roundUpToOdd(const Integer& that);
	
	//! Rounds up to the next odd number.
	/*!
	Preconditions:
	Real is a native floating point type.

	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Real, EnableIf<std::is_floating_point<Real>> = 0>
	integer roundUpToOdd(const Real& that);

	//! Rounds up to the next even number.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Integer>
	Integer roundUpToEven(const Integer& that);

	//! Rounds up to the next even number.
	/*!
	Preconditions:
	Real is a native floating point type.

	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Real, EnableIf<std::is_floating_point<Real>> = 0>
	integer roundUpToEven(const Real& that);

	//! Rounds up to the next power of 2 (unsigned integers).
	/*!
	Preconditions:
	Integer is an unsigned native integer.

	Time complexity: O(log(sizeInBits<Integer>()) + 1)
	Exception safety: nothrow
	*/
	template <typename Integer, EnableIf<std::is_unsigned<Integer>> = 0>
	Integer roundUpToPowerOfTwo(const Integer& that);

	//! Rounds up to the next power of 2 (signed integers).
	/*!
	Preconditions:
	Integer is a signed native integer.
	The next power of 2 can be represented.

	Time complexity: O(log(sizeInBits<Integer>()) + 1)
	Exception safety: nothrow
	*/
	template <typename Integer, EnableIf<std::is_signed<Integer>> = 0>
	Integer roundUpToPowerOfTwo(const Integer& that);

	//! Rounds up 'that' to the next multiple of power of 2.
	/*!
	Preconditions:
	!negative(power)

	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Integer>
	Integer roundUpToPowerOfTwo(
		const Integer& that, 
		integer power);

	//! Returns ceil(divide / byThis).
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Integer>
	Integer divideAndRoundUp(
		const Integer& divide, 
		const Integer& byThis);

}

#include "pastel/sys/rounding.hpp"

#endif
