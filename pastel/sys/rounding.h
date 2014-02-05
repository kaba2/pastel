// Description: Rounding
// Documentation: math_functions.txt

#ifndef PASTELSYS_ROUNDING_H
#define PASTELSYS_ROUNDING_H

#include "pastel/sys/mytypes.h"

#include <type_traits>

namespace Pastel
{

	//! Rounds up 'that' to the next multiple of 'to'.
	/*!
	Preconditions:
	!negative(to)

	Time complexity: constant
	Exception safety: nothrow
	*/
	template <typename Integer>
	Integer roundUpTo(
		const Integer& that, const Integer& to);

	//! Rounds up 'that' to the next odd number.
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/
	template <typename Integer>
	Integer roundUpToOdd(const Integer& that);
	
	template <typename Real>
	PASTEL_ENABLE_IF(std::is_floating_point<Real>, integer)
		roundUpToOdd(const Real& that);

	//! Rounds up 'that' to the next even number.
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/
	template <typename Integer>
	Integer roundUpToEven(const Integer& that);

	template <typename Real>
	PASTEL_ENABLE_IF(std::is_floating_point<Real>, integer)
		roundUpToEven(const Real& that);

	//! Rounds up 'that' to the next power of 2.
	/*!
	Preconditions:
	that >= 0
	sizeof(integer) <= 8

	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpToPowerOfTwo(integer that);

	//! Rounds up 'that' to the next multiple of power of 2.
	/*!
	Preconditions:
	!negative(power)

	Time complexity: constant
	Exception safety: nothrow
	*/
	template <typename Integer>
	Integer roundUpToPowerOfTwo(
		const Integer& that, 
		integer power);

	//! Returns ceil(divide / byThis).
	template <typename Integer>
	Integer divideAndRoundUp(
		const Integer& divide, 
		const Integer& byThis);

}

#include "pastel/sys/rounding.hpp"

#endif
