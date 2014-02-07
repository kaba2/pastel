#ifndef PASTELSYS_SET_BITS_HPP
#define PASTELSYS_SET_BITS_HPP

#include "pastel/sys/set_bits.h"
#include "pastel/sys/bitmask.h"

namespace Pastel
{

	template <typename Integer>
	PASTEL_ENABLE_IF(std::is_unsigned<Integer>, Integer)
		setBits(
		const Integer& that,
		integer begin,
		integer end,
		bool value)
	{
		PENSURE_OP(begin, >=, 0);
		PENSURE_OP(begin, <=, end);
		PENSURE_OP(end, <=, bits(that));

		Integer result = that;
		if (value)
		{
			result |= bitMask<Integer>(begin, end);
		}
		else
		{
			result &= ~bitMask<Integer>(begin, end);
		}

		return result;
	}
	
}

#endif
