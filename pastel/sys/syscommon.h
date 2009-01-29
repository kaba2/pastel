/*!
\file
\brief Miscellaneous commonly needed tools.
*/

#ifndef PASTELSYS_SYSCOMMON_H
#define PASTELSYS_SYSCOMMON_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

#include <vector>
#include <string>

#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	//! Reverses the order of bits.

	uint32 reverseBits(uint32 input);

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
	Preconditions:
	to >= 0

	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpToOdd(integer that);

	//! Rounds up 'that' to the next even number.
	/*!
	Preconditions:
	to >= 0

	Time complexity: constant
	Exception safety: nothrow
	*/
	integer roundUpToEven(integer that);

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

	//! Converts a real in [0, 1] to integer in [0, max].
	/*!
	Numbers < 0 will be converted to 0.
	Numbers > 1 will be converted to 'max'.
	*/

	integer realToInteger(real number, integer max);

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
