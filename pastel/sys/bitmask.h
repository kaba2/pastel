// Description: Bit masks
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_BITMASK_H
#define PASTELSYS_BITMASK_H

#include "pastel/sys/integer_concept.h"

namespace Pastel
{

	//! Returns an integer with a single bit set.
	/*!
	Preconditions:
	bit >= 0

	returns:
	An integer in whose bit-representation the
	'bit' bit is one, while the other bits are zero.
	*/
	template <typename Integer>
	Integer singleBitMask(integer bit);

	//! Returns an integer with lower bits set.
	/*!
	Preconditions:
	endBit >= 0

	returns:
	An integer in whose bit-representation the
	bits in the range [0, endBit) are one, 
	while the other bits are zero.
	*/
	template <typename Integer>
	Integer bitMask(integer endBit);

	//! Returns an interval of bits set.
	/*!
	Preconditions:
	beginBit >= 0
	beginBit <= endBit

	returns:
	An integer in whose bit-representation the
	bits in the range [beginBit, endBit) are one, 
	while the other bits are zero.
	*/
	template <typename Integer>
	Integer bitMask(integer beginBit, integer endBit);

}

#include "pastel/sys/bitmask.hpp"

#endif