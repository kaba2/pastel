// Description: Sets the bits below the first 1-bit
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_FLIP_LEADING_ZERO_BITS_H
#define PASTELSYS_FLIP_LEADING_ZERO_BITS_H

#include "pastel/sys/integer_concept.h"

namespace Pastel
{

	//! Sets the bits below the first 1-bit.
	/*!
	Time complexity: constant, provided the basic
	operations on integers are.

	returns:
	An integer which is otherwise equal to that, but 
	has the leading zero bits changed to 1;
	x..x10..0 --> x..x11..1
	*/
	template <typename Integer>
	Integer flipLeadingZeroBits(const Integer& that);

}

#include "pastel/sys/flip_leading_zero_bits.hpp"

#endif

