// Description: Rounding
// Documentation: math_functions.txt

#ifndef PASTELSYS_ROUNDING_H
#define PASTELSYS_ROUNDING_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Rounds up 'that' to the next multiple of 'to'.
	/*!
	Preconditions:
	that >= 0
	to >= 0

	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpTo(integer that, integer to);

	//! Rounds up 'that' to the next odd number.
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpToOdd(integer that);
	
	integer roundUpToOdd(real that);

	//! Rounds up 'that' to the next even number.
	/*!
	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpToEven(integer that);

	integer roundUpToEven(real that);

	//! Rounds up 'that' to the next power of 2.
	/*!
	Preconditions:
	that >= 0
	sizeof(integer) <= 8

	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpToPowerOf2(integer that);

	//! Rounds up 'that' to the next multiple of power of 2.
	/*!
	Preconditions:
	that >= 0
	power >= 0

	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpToPowerOf2(integer that, integer power);

	//! Returns ceil(divide / byThis).
	template <typename Integer>
	Integer divideAndRoundUp(const Integer& divide, const Integer& byThis);

}

#include "pastel/sys/rounding.hpp"

#endif
