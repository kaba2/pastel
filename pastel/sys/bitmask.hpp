#ifndef PASTELSYS_BITMASK_HPP
#define PASTELSYS_BITMASK_HPP

#include "pastel/sys/bitmask.h"

namespace Pastel
{

	template <typename Integer>
	Integer singleBitMask(integer bit)
	{
		PENSURE_OP(bit, >=, 0);

		return (Integer)1 << bit;
	}

	template <typename Integer>
	Integer bitMask(integer endBit)
	{
		PENSURE_OP(endBit, >=, 0);

		return ((Integer)1 << endBit) - (Integer)1;
	}

	template <typename Integer>
	Integer bitMask(integer beginBit, integer endBit)
	{
		PENSURE_OP(beginBit, >=, 0);
		PENSURE_OP(beginBit, <=, endBit);

		return ((Integer)1 << endBit) - (((Integer)1 << beginBit));
	}

}

#endif
