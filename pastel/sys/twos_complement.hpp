// Description: Two's complement
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_TWOS_COMPLEMENT_HPP
#define PASTELSYS_TWOS_COMPLEMENT_HPP

#include "pastel/sys/twos_complement.h"
#include "pastel/sys/bitmask.h"

namespace Pastel
{

	template <typename Integer>
	PASTEL_ENABLE_IF(
		std::is_signed<Integer>, 
		typename std::make_unsigned<Integer>::type)  
		signedToTwosComplement(const Integer& that)
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

		using Unsigned = std::make_unsigned<Integer>::type;
		return (that >= 0) ? (Unsigned)that : -((Unsigned)(-that));
	}

	template <typename Finite_Integer>
	PASTEL_ENABLE_IF(
		std::is_unsigned<Finite_Integer>, 
		typename std::make_signed<Finite_Integer>::type)  
		twosComplementToSigned(const Finite_Integer& that)
	{
		// See the signedToTwosComplement() for documentation.
		using Signed = std::make_signed<Finite_Integer>::type;
		return twosComplementNegative(that) ? -((Signed)(-that)) : (Signed)that;
	}

	template <typename Finite_Integer>
	PASTEL_ENABLE_IF(std::is_unsigned<Finite_Integer>, bool)  
		twosComplementNegative(const Finite_Integer& that)
	{
		// A two's complement integer is negative if and only if
		// its most-significant bit is 1.
		return (that & singleBitMask<Finite_Integer>(bits(that) - 1)) != 0;
	}

	template <typename Finite_Integer>
	PASTEL_ENABLE_IF(std::is_unsigned<Finite_Integer>, Finite_Integer)  
		arihmeticShiftRight(
			const Finite_Integer& that, 
			integer n)
	{
		PENSURE_OP(n, >=, 0);

		if (n >= bits(that))
		{
			// The right-shift by bits not less than
			// the number of bits in Finite_Integer
			// is implementation-defined by the C++ standard.
			// We define it as zero.
			return 0;
		}

		// On unsigned integers, the right shift pushes
		// 0-bits to the most-significant bits. This is
		// what we want when 'that' represents a non-negative
		// value. However, when 'that' represents a negative
		// value, then we want the arithmetic right-shift
		// to push 1-bits to the most-significant bits.
		// This is achieved by first complementing, then
		// shifting, and then complementing again.

		return twosComplementNegative(that) ?
			~((~that) >> n) : (that >> n);
	}

	template <typename Finite_Integer>
	PASTEL_ENABLE_IF(std::is_unsigned<Finite_Integer>, Finite_Integer)  
		shiftRight(
			const Finite_Integer& that, 
			integer n)
	{
		PENSURE_OP(n, >=, 0);

		if (n >= bits(that))
		{
			// The right-shift by bits not less than
			// the number of bits in Finite_Integer
			// is implementation-defined by the C++ standard.
			// We define it as zero.
			return 0;
		}

		return that >> n;
	}

	template <typename Finite_Integer>
	PASTEL_ENABLE_IF(std::is_signed<Finite_Integer>, Finite_Integer)  
		shiftRight(
			const Finite_Integer& that, 
			integer n)
	{
		// The right-shift for signed native integers
		// is implementation-defined. Thus for portability
		// we have to simulate it via the unsigned integers.

		if (that < 0) 
		{
			// By the C++ standard, negation for unsigned integers
			// computes the two's complement.
			return twosComplementToSigned(
				-shiftRight(-signedToTwosComplement(that)));
		}

		return twosComplementToSigned(
			shiftRight(signedToTwosComplement(that)));
	}

}

#endif
