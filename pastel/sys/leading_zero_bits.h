// Description: Number of leading zero bits
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_LEADING_ZERO_BITS_H
#define PASTELSYS_LEADING_ZERO_BITS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/finite_integer_concept.h"

namespace Pastel
{

	//! Returns the number of leading zero bits.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Finite_Integer>
	integer leadingZeroBits(const Finite_Integer& that);

}

#include "pastel/sys/leading_zero_bits.hpp"

#endif
