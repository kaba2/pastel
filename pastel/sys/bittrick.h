// Description: Bit tricks

#ifndef PASTEL_BITTRICK_H
#define PASTEL_BITTRICK_H

#include "pastel/sys/integer_concept.h"

namespace Pastel
{

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
	PASTEL_ENABLE_IF_C((ToBits < FromBits), Integer)
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	PASTEL_ENABLE_IF_C((ToBits == FromBits), Integer)
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	PASTEL_ENABLE_IF_C((ToBits > FromBits && ToBits <= 2 * FromBits), Integer)
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	PASTEL_ENABLE_IF_C((ToBits > 2 * FromBits && ToBits <= 3 * FromBits), Integer)
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	PASTEL_ENABLE_IF_C((ToBits > 3 * FromBits && ToBits <= 4 * FromBits), Integer)
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

	template <typename Integer>
	Integer extractBits(const Integer& number, 
		integer fromBit, integer bits);

}

#include "pastel/sys/bittrick.hpp"

#endif
