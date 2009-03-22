/*!
\file
\brief Miscellaneous commonly needed tools.
*/

#ifndef PASTELSYS_SYSCOMMON_H
#define PASTELSYS_SYSCOMMON_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"
#include "pastel/sys/point.h"

#include <vector>
#include <string>

#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	/*!
	Example:
	Given a 2d horizontal line segment [xMin, xMax[ on row y,
	xMin, xMax real numbers, y an integer.
	Which pixels should be set? Answer:
	[toPixelSpanPoint(xMin), toPixelSpanPoint(xMax)[
	on row y.
	*/
	integer toPixelSpanPoint(real t);

	template <int N>
	Point<N, integer> toPixelSpanPoint(
		const Point<N, real>& that);

	//! Squares the given variable.

	template <typename Type>
	Type square(const Type& that);

	//! Clamps the variable to a given range.
	/*!
	Preconditions:
	xMin <= xMax
	*/

	template <typename Type>
	Type clamp(const Type& x,
		const PASTEL_NO_DEDUCTION(Type)& xMin,
		const PASTEL_NO_DEDUCTION(Type)& xMax);

	//! Returns true on an odd integer, false otherwise.

	template <typename Integer>
	inline bool odd(const Integer& x);

	//! Returns true on an even integer, false otherwise.

	template <typename Integer>
	inline bool even(const Integer& x);

	//! Computes the remainder
	/*!
	Preconditions:
	n > 0

	mod(-1, n) = n - 1, and so on
	*/
	inline integer mod(integer x, integer n);

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

	//! Returns if 'that' is a power of 2.
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

	template <int N>
	real64 ditheredDequantizeSigned(integer i);

	//! Converts a real [-1, 1] to integer [-(2^(N - 1) - 1) , 2^(N - 1) - 1].
	/*!
	Real values below the range will be converted to -(2^(N - 1) - 1).
	Real values above the range will be converted to 2^(N - 1) - 1.
	*/

	template <int N>
	integer quantizeSigned(real64 r);

	//! Scales an integer from [0, 2^FromBits - 1] to [0, 2^ToBits - 1].
	/*!
	Preconditions:
	FromBits > 0
	ToBits > 0
	ToBits <= 4 * FromBits

	The last requirement can be extended by implementing
	new functions for the larger ranges.

	The computation that is done is effectively:
	round[(number * (2^ToBits - 1)) / (2^(FromBits) - 1)]
	This computation would be a bit expensive if
	evaluated as it is. However, it turns out
	that you can compute this quantity very efficiently
	by using integer shifts and additions alone.
	*/

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits < FromBits), Integer>::type
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits == FromBits), Integer>::type
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits > FromBits && ToBits <= 2 * FromBits), Integer>::type
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits > 2 * FromBits && ToBits <= 3 * FromBits), Integer>::type
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits > 3 * FromBits && ToBits <= 4 * FromBits), Integer>::type
		scaleInteger(const Integer& number);

	//! Returns the bits at range [FromBit, FromBit + Bits[.
	/*!
	Preconditions:
	FromBit > 0
	Bits > 0
	Bits <= 32

	The computation is defined by:
	(number >> FromBit) & (1 << Bits - 1);
	*/

	template <int FromBit, int Bits, typename Integer>
	Integer extractBits(const Integer& number);

}

#include "pastel/sys/syscommon.hpp"

#endif
