// Description: More common math functions

#ifndef PASTEL_MATH_FUNCTIONS_MORE_H
#define PASTEL_MATH_FUNCTIONS_MORE_H

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	//! Squares the given variable.
	/*!
	[Squaring]
	*/
	template <typename Type>
	Type square(const Type& that);

	//! Clamps the variable to a given range.
	/*!
	[Clamping]

	Preconditions:
	xMin <= xMax
	*/

	template <typename Type>
	Type clamp(const Type& x,
		const PASTEL_NO_DEDUCTION(Type)& xMin,
		const PASTEL_NO_DEDUCTION(Type)& xMax);

	//! Returns true on an [odd integer], false otherwise.

	template <typename Integer>
	inline bool odd(const Integer& x);

	//! Returns true on an [even integer], false otherwise.

	template <typename Integer>
	inline bool even(const Integer& x);

	//! Computes the [remainder].
	/*!
	Preconditions:
	n > 0
	Postconditions:
	0 <= x < n

	Returns:
	x mod n

	mod(-1, n) = n - 1, and so on
	*/
	inline integer mod(integer x, integer n);

	//! Returns x mod 1.
	/*!
	Postconditions:
	0 <= x < 1

	Returns:
	x - floor(x)
	*/
	inline real mod(real x);

	//! Returns x mod n.
	/*!
	Preconditions:
	n > 0
	Postconditions:
	0 <= x < n

	This is a convenience function that calls
	mod(x / n) * n
	*/
	inline real mod(real x, real n);

	//! Returns floor(log_2(that)).
	/*!
	Preconditions:
	that >= 0
	to >= 0

	Time complexity: constant
	Exception safety: nothrow
	*/
	integer integerLog2(integer that);

	//! Returns if 'that' [is a power of 2].
	/*!
	Preconditions:
	that >= 0

	Time complexity: constant
	Exception safety: nothrow
	*/
	bool isPowerOfTwo(integer that);

	template <typename Real>
	void realToReal(
		const Real& x,
		const PASTEL_NO_DEDUCTION(Real)& fromMin, 
		const PASTEL_NO_DEDUCTION(Real)& fromMax,
		const PASTEL_NO_DEDUCTION(Real)& toMin, 
		const PASTEL_NO_DEDUCTION(Real)& toMax);

}

#endif
