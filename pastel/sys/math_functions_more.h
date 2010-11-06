// Description: More common math functions
// Documentation: basic_math.txt

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

	//! Rounds up 'that' to the next multiple of 'to'.
	/*!
	[Round up to a multiple]

	Preconditions:
	that >= 0
	to >= 0

	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpTo(integer that, integer to);

	//! Rounds up 'that' to the next odd number.
	/*!
	[Round up to odd]

	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpToOdd(integer that);
	
	integer roundUpToOdd(real that);

	//! Rounds up 'that' to the next even number.
	/*!
	[Round up to even]

	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpToEven(integer that);

	integer roundUpToEven(real that);

	//! Rounds up 'that' to the next power of 2.
	/*!
	[Round up to power of 2]

	Preconditions:
	that >= 0

	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpToPowerOf2(integer that);

	//! Rounds up 'that' to the next multiple of power of 2.
	/*!
	[Round up to multiple of power of 2]

	Preconditions:
	that >= 0
	power >= 0

	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpToPowerOf2(integer that, integer power);

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

	//! Converts real [0, 1] to integer [0, numbers - 1].
	/*!
	[Quantization]

	x < 0 will be converted to 0.
	x > 1 will be converted to numbers - 1.

	Random uniform distributions on [0, 1] map
	to random uniform distributions on [0, numbers - 1].
	*/
	integer quantizeUnsigned(real64 x, integer numbers);

	//! Converts integer [0, numbers - 1] to real [0, 1].
	/*!
	x < 0 will be converted to 0.
	x > numbers - 1 will be converted to 1.

	0 maps to 0.
	numbers - 1 maps to 1.
	*/

	real64 dequantizeUnsignedMatchEnds(integer x, integer numbers);

	//! Converts integer [0, numbers - 1] to real [0, 1].
	/*!
	x < 0 will be converted to 0.
	x > numbers - 1 will be converted to 1.

	Error is minimized.
	*/

	real64 dequantizeUnsigned(integer x, integer numbers);

	//! Converts an integer [-2^(N - 1) , 2^(N - 1) - 1] to a real [-1, 1].
	/*!
	Integer values below the range will be converted to -1.
	Integer values above the range will be converted to 1.
	*/

	template <int N>
	real64 dequantizeSigned(integer i);

	//! Converts an integer [-2^(N - 1) , 2^(N - 1) - 1] to a real [-1, 1].
	/*!
	Integer values below the range will be converted to -1.
	Integer values above the range will be converted to 1.
	Random dithering is applied to the result.
	*/

	/*
	template <int N>
	real64 ditheredDequantizeSigned(integer i);
	*/

	//! Converts a real [-1, 1] to integer [-(2^(N - 1) - 1) , 2^(N - 1) - 1].
	/*!
	Real values below the range will be converted to -(2^(N - 1) - 1).
	Real values above the range will be converted to 2^(N - 1) - 1.
	*/

	template <int N>
	integer quantizeSigned(real64 r);

}

#endif
