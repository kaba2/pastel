// Description: Bit extraction

#ifndef PASTELSYS_EXTRACT_BITS_H
#define PASTELSYS_EXTRACT_BITS_H

#include "pastel/sys/integer_concept.h"

namespace Pastel
{

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

	template <typename Integer>
	Integer extractBits(const Integer& number, 
		integer fromBit, integer bits);

}

#include "pastel/sys/extract_bits.hpp"

#endif
