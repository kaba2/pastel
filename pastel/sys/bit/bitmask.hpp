#ifndef PASTELSYS_BITMASK_HPP
#define PASTELSYS_BITMASK_HPP

#include "pastel/sys/bit/bitmask.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/integer/finite_integer_concept.h"

namespace Pastel
{

	template <
		typename Finite_Integer,
		Requires<
			Models<Finite_Integer, Finite_Integer_Concept>
		>
	>
	Finite_Integer singleBitMask(integer bit)
	{
		PENSURE_OP(bit, >=, 0);

		// C++ standard:
		// If E1 has an unsigned type, the value of the result is 
		// E1 × 2^E2, reduced modulo one more than the maximum value 
		// representable in the result type. 

		// So the shift is defined even when endBit equals or exceeds 
		// the number of bits.

		// However, such a left-shift used to be implementation-defined.
		// Indeed, Visual Studio 2013 still gives a warning about this.

		if (bit >= bits(Finite_Integer()))
		{
			return 0;
		}

		return (Finite_Integer)1 << bit;
	}

	template <
		typename Finite_Integer,
		Requires<
			Models<Finite_Integer, Finite_Integer_Concept>
		>
	>
	Finite_Integer bitMask(integer endBit)
	{
		PENSURE_OP(endBit, >=, 0);

		if (endBit >= bits(Finite_Integer()))
		{
			return -1;
		}

		// See the documentation for the singleBitMask().
		return ((Finite_Integer)1 << endBit) - (Finite_Integer)1;
	}

	template <
		typename Finite_Integer,
		Requires<
			Models<Finite_Integer, Finite_Integer_Concept>
		>
	>
	Finite_Integer bitMask(integer beginBit, integer endBit)
	{
		PENSURE_OP(beginBit, >=, 0);
		PENSURE_OP(beginBit, <=, endBit);

		// See the documentation for the singleBitMask().
		return bitMask<Finite_Integer>(endBit) ^ bitMask<Finite_Integer>(beginBit);
	}

}

#endif
