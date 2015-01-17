// Description: Number of leading one bits
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_LEADING_ONE_BITS_H
#define PASTELSYS_LEADING_ONE_BITS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/integer/finite_integer_concept.h"

namespace Pastel
{

	//! Returns the number of leading one bits.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Finite_Integer>
	integer leadingOneBits(const Finite_Integer& that);

}

#include "pastel/sys/bit/leading_one_bits.hpp"

#endif
