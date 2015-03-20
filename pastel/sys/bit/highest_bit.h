// Description: Index of the highest bit
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_HIGHEST_BIT_H
#define PASTELSYS_HIGHEST_BIT_H

#include "pastel/sys/integer/finite_integer_concept.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns the index of the highest 1-bit of the given integer.
	/*!
	returns:
	The index of the highest 1-bit, if non-zero.
	Otherwise -1.
	*/
	template <typename Finite_Integer>
	integer highestBit(const Finite_Integer& data);

}

#include "pastel/sys/bit/highest_bit.hpp"

#endif
