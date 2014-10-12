// Description: Zeros the bits below the first 0-bit
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_FLIP_LEADING_ONE_BITS_H
#define PASTELSYS_FLIP_LEADING_ONE_BITS_H

#include "pastel/sys/finite_integer_concept.h"

#include <type_traits>

namespace Pastel
{

	//! Zeros the bits below the first 0-bit.
	/*!
	Preconditions:
	Finite_Integer is not a signed integer.
	This is required to avoid portability issues
	with bitwise operations on signed integers.

	Time complexity: constant, provided the basic
	operations on integers are.

	returns:
	An integer which is otherwise equal to that, but 
	has the leading one bits changed to 0;
	x..x01..1 --> x..x00..0
	*/
	template <
		typename Finite_Integer,
		DisableIf<std::is_signed<Finite_Integer>> = 0>
	Finite_Integer flipLeadingOneBits(const Finite_Integer& that);

}

#include "pastel/sys/flip_leading_one_bits.hpp"

#endif

