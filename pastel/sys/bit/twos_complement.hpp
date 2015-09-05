#ifndef PASTELSYS_TWOS_COMPLEMENT_HPP
#define PASTELSYS_TWOS_COMPLEMENT_HPP

#include "pastel/sys/bit/twos_complement.h"
#include "pastel/sys/bit/bitmask.h"
#include "pastel/sys/bit/set_bits.h"

namespace Pastel
{

	template <typename Integer, Requires<std::is_signed<Integer>>>
	typename std::make_unsigned<Integer>::type
	signedToTwosComplement(const Integer& that)
	{
		// C++ standard
		// (Standard conversions --> Integral conversions):

		// If the destination type is unsigned, the resulting 
		// value is the least unsigned integer congruent to the 
		// source integer (modulo 2^n where n is the number of 
		// bits used to represent the unsigned type). [ Note: In 
		// a two’s complement representation, this conversion is 
		// conceptual and there is no change in the bit pattern 
		// (if there is no truncation). --- end note 

		using Unsigned = typename std::make_unsigned<Integer>::type;
		return (Unsigned)that;
	}

	template <typename Finite_Integer, Requires<std::is_unsigned<Finite_Integer>>>
	typename std::make_signed<Finite_Integer>::type
	twosComplementToSigned(const Finite_Integer& that)
	{
		// C++ standard
		// (Standard conversions --> Integral conversions):

		// "If the destination type is signed, the value is 
		// unchanged if it can be represented in the destination 
		// type (and bit-ﬁeld width); otherwise, the value is 
		// implementation-deﬁned."

		// "Unsigned integers, declared unsigned, shall obey the 
		// laws of arithmetic modulo 2^n where n is the number of 
		// bits in the value representation of that particular 
		// size of integer.""

		// Due to modulo arithmetic the negative of an unsigned 
		// integer is its two's complement. 

		// The general strategy is to use the native conversion 
		// for non-negative numbers, and two's complement-convert-negate
		// for negative numbers.

		// The only problematic case is the two's complement
		// form 2^{n - 1}, which corresponds to -2^{n - 1}
		// (e.g. 128 for n = 8). Let us analyze this using 
		// some concrete examples.
		//
		// 1) If the signed representation is two's
		// complement, then we can do 
		// 
		//     (Signed)(-(that - 1)) - 1
		//
		// although this is probably unnecessary;
		// it would be odd for the implementation to 
		// do anything else than map 2^{n - 1} to -2^{n - 1}
		// (i.e. preserve the bits, just change their
		// interpretation).
		//
		// 2) If the signed representation is sign
		// magnitude, then the number can not be 
		// represented.
		//
		// 3) If the signed representation is ones'
		// complement, then the number can not
		// be represented.
		//
		// The essential problem is that the two's complement
		// form can represent one number more than the other
		// two representations.
	
		using Signed = typename std::make_signed<Finite_Integer>::type;
		if (twosComplementNegative(that))
		{
			// This is equivalent to that != 2^{n - 1}
			// (because zero is not negative).
			//PENSURE(that != -that);

			return -((Signed)(-that));
		}

		return (Signed)that;
	}

	template <typename Finite_Integer, Requires<std::is_unsigned<Finite_Integer>>>
	bool twosComplementNegative(const Finite_Integer& that)
	{
		// A two's complement integer is negative if and only if
		// its most-significant bit is 1.
		return (that & singleBitMask<Finite_Integer>(bits(that) - 1)) != 0;
	}

	template <typename Finite_Integer, Requires<std::is_unsigned<Finite_Integer>>>
	Finite_Integer arithmeticShiftRight(
			const Finite_Integer& that, 
			integer n)
	{
		PENSURE_OP(n, >=, 0);

		if (n >= bits(that))
		{
			// The right-shift by bits not less than
			// the number of bits in Finite_Integer
			// is implementation-defined by the C++ standard.
			// We think of the integer as an infinite sequence
			// where the sign bit is replicated.
			return twosComplementNegative(that) ? 
				-1 : 0;
		}

		// On unsigned integers, the right shift pushes
		// 0-bits to the most-significant bits. This is
		// what we want when 'that' represents a non-negative
		// value. 
		Finite_Integer result = that >> n;
		if (twosComplementNegative(that))
		{
			// However, when 'that' represents a negative
			// value, then we want the arithmetic right-shift
			// to push 1-bits to the most-significant bits.
			result = setBits(result, bits(that) - n, bits(that));
		}
		return result;
	}

	template <typename Finite_Integer, Requires<std::is_unsigned<Finite_Integer>>>
	Finite_Integer shiftRight(
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

	template <typename Finite_Integer, Requires<std::is_signed<Finite_Integer>>>
	Finite_Integer shiftRight(
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
