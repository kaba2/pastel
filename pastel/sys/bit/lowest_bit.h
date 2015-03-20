// Description: Index of the lowest bit
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_LOWEST_BIT_H
#define PASTELSYS_LOWEST_BIT_H

#include "pastel/sys/integer/finite_integer_concept.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns the index of the lowest 1-bit of the given integer.
	/*!
	returns:
	The index of the lowest 1-bit, if non-zero.
	Otherwise -1.
	*/
	template <typename Finite_Integer>
	integer lowestBit(const Finite_Integer& data);

}

#include "pastel/sys/bit/lowest_bit.hpp"

#endif
