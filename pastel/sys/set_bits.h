// Description: Sets bits in an unsigned integer.
// Documentation: bit_tricks.h

#ifndef PASTELSYS_SET_BITS_H
#define PASTELSYS_SET_BITS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/integer_concept.h"

#include <type_traits>

namespace Pastel
{

	//! Sets the bits in the range [begin, end) for an unsigned integer.
	/*!
	Preconditions:
	0 <= begin <= end
	*/
	template <typename Integer>
	PASTEL_ENABLE_IF(std::is_unsigned<Integer>, Integer)
		setBits(
		const Integer& that,
		integer begin,
		integer end,
		bool value);

}

#include "pastel/sys/set_bits.hpp"

#endif
