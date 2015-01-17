// Description: Zero bits above the first 1-bit
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_ZERO_HIGHER_BITS_H
#define PASTELSYS_ZERO_HIGHER_BITS_H

#include "pastel/sys/integer/integer_concept.h"

namespace Pastel
{

	//! Zeros bits above the first 1-bit.
	/*!
	Time complexity: constant, provided the basic
	operations on integers are.

	returns:
	Zero if 'that' is zero. Otherwise,
	an integer which is a power-of-two and has the
	same amount of leading 0-bits as 'that';
	x..x10..0 --> 0..010..0.
	*/
	template <typename Integer>
	Integer zeroHigherBits(const Integer& that);

}

#include "pastel/sys/bit/zero_higher_bits.hpp"

#endif

