// Description: Zeros the bits below the first 0-bit
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_FLIP_LEADING_ONE_BITS_H
#define PASTELSYS_FLIP_LEADING_ONE_BITS_H

#include "pastel/sys/integer_concept.h"

namespace Pastel
{

	//! Zeros the bits below the first 0-bit.
	/*!
	Time complexity: constant, provided the basic
	operations on integers are.

	returns:
	An integer which is otherwise equal to that, but 
	has the leading one bits changed to 0;
	x..x01..1 --> x..x00..0
	*/
	template <typename Integer>
	Integer flipLeadingOneBits(const Integer& that);

}

#include "pastel/sys/flip_leading_one_bits.hpp"

#endif

