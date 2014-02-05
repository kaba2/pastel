// Description: Two's complement
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_TWOS_COMPLEMENT_HPP
#define PASTELSYS_TWOS_COMPLEMENT_HPP

#include "pastel/sys/twos_complement.h"
#include "pastel/sys/bitmask.h"

namespace Pastel
{

	template <typename Finite_Integer>
	PASTEL_ENABLE_IF(
		std::is_signed<Finite_Integer>, 
		typename std::make_unsigned<Finite_Integer>::type)  
		signedToTwosComplement(const Finite_Integer& that)
	{
		// Here's the relevant pieces from the C++ standard:

		// The range of nonnegative values of a signed integer 
		// type is a subrange of the corresponding unsigned 
		// integer type, and the value representation of each 
		// corresponding signed/unsigned type shall be the same.

		// Unsigned integers, declared unsigned, shall obey the 
		// laws of arithmetic modulo 2^n where n is the number of 
		// bits in the value representation of that particular 
		// size of integer.

		// Therefore the negative of an unsigned integer is
		// its two's complement. Also, if that < 0, then its
		// negation will fit into the corresponding unsigned
		// integer.

		using Unsigned = std::make_unsigned<Finite_Integer>::type;
		return (that >= 0) ? (Unsigned)that : -((Unsigned)(-that));
	}

	template <typename Finite_Integer>
	PASTEL_ENABLE_IF(
		std::is_unsigned<Finite_Integer>, 
		typename std::make_signed<Finite_Integer>::type)  
		twosComplementToSigned(const Finite_Integer& that)
	{
		// See the signedToTwosComplement() for documentation.

		// In two's complement form, an integer is 
		// non-negative if and only if the last bit is 0.
		bool nonNegative = 
			((that & singleBitMask<Finite_Integer>(bits(that) - 1)) == 0);

		using Signed = std::make_signed<Finite_Integer>::type;
		return nonNegative ? (Signed)that : -((Signed)(-that));
	}

}

#endif
