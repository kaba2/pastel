#ifndef PASTELSYS_EXTRACT_BITS_HPP
#define PASTELSYS_EXTRACT_BITS_HPP

#include "pastel/sys/extract_bits.h"

namespace Pastel
{

	template <int FromBit, int Bits, typename Integer>
	Integer extractBits(const Integer& number)
	{
		PASTEL_STATIC_ASSERT(FromBit >= 0);
		PASTEL_STATIC_ASSERT(Bits > 0);

		return (number >> FromBit) & ((1 << Bits) - 1);
	}

	template <typename Integer>
	Integer extractBits(const Integer& number, 
		integer fromBit, integer bits)
	{
		PENSURE_OP(fromBit, >=, 0);
		PENSURE_OP(bits, >, 0);

		return (number >> fromBit) & ((1 << bits) - 1);
	}

}

#endif
