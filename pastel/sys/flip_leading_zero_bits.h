// Description: Sets the bits below the first 1-bit
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_FLIP_LEADING_ZERO_BITS_H
#define PASTELSYS_FLIP_LEADING_ZERO_BITS_H

#include "pastel/sys/finite_integer_concept.h"

#include <type_traits>

namespace Pastel
{

	//! Sets the bits below the first 1-bit.
	/*!
	Preconditions:
	Finite_Integer is not a signed integer.
	This is required to avoid portability issues
	with bitwise operations on signed integers.

	Time complexity: constant, provided the basic
	operations on integers are.

	returns:
	An integer which is otherwise equal to that, but 
	has the leading zero bits changed to 1;
	x..x10..0 --> x..x11..1
	*/
	template <typename Finite_Integer>
	PASTEL_DISABLE_IF(std::is_signed<Finite_Integer>, Finite_Integer) 
		flipLeadingZeroBits(const Finite_Integer& that);

}

#include "pastel/sys/flip_leading_zero_bits.hpp"

#endif

