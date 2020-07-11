// Description: Two's complement

#ifndef PASTELSYS_TWOS_COMPLEMENT_H
#define PASTELSYS_TWOS_COMPLEMENT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/integer/finite_integer_concept.h"
#include "pastel/sys/integer/integer_concept.h"
#include "pastel/sys/bit/bitmask.h"
#include "pastel/sys/bit/set_bits.h"

#include <type_traits>

namespace Pastel
{

	//! Returns whether 'that' represents a negative value.
	/*!
	A two's complement integer is negative if and only if
	its most-significant bit is 1.
	*/
	template <typename Finite_Integer> 
	requires std::is_unsigned_v<Finite_Integer>
	bool twosComplementNegative(const Finite_Integer& that)
	{
		// A two's complement integer is negative if and only if
		// its most-significant bit is 1.
		return (that & singleBitMask<Finite_Integer>(bits(that) - 1)) != 0;
	}

	//! Two's complement form from a signed integer
	/*!
	Preconditions:
	Integer is a signed native integer.

	The bit-presentation of a signed integer is implementation-defined,
	and so not portable. However, the bit-representation of an unsigned
	integer is well-defined. This function converts the signed integer,
	no matter what its bit-presentation, portably to its two's complement
	form, and stores it in the bits of the corresponding unsigned integer 
	type.
	*/	
	template <typename Integer>
	requires std::is_signed_v<Integer>
	typename std::make_unsigned<Integer>::type 
	signedToTwosComplement(const Integer& that)
	{
		// C++ standard
		// (Standard conversions --> Integral conversions):

		// If the destination type is unsigned, the resulting 
		// value is the least unsigned integer congruent to the 
		// source integer (modulo 2^n where n is the number of 
		// bits used to represent the unsigned type). [ Note: In 
		// a two's complement representation, this conversion is 
		// conceptual and there is no change in the bit pattern 
		// (if there is no truncation). --- end note 

		using Unsigned = typename std::make_unsigned<Integer>::type;
		return (Unsigned)that;
	}

	//! Signed integer from a two's complement form.
	/*!
	Preconditions:
	Finite_Integer is an unsigned native integer.
	that != 2^{n - 1})

	This is the inverse of signedToTwosComplement(), see its
	documentation. However, this function can not portably be 
	defined for the two's complement form 2^{n - 1}, where n is 
	the number of bits: in ones' complement and sign-magnitude
	representations the corresponding number -2^{n - 1} does 
	not exist.
	*/	
	template <typename Finite_Integer> 
	requires std::is_unsigned_v<Finite_Integer>
	typename std::make_signed<Finite_Integer>::type
		twosComplementToSigned(const Finite_Integer& that)
	{
		// C++ standard
		// (Standard conversions --> Integral conversions):

		// "If the destination type is signed, the value is 
		// unchanged if it can be represented in the destination 
		// type (and bit-field width); otherwise, the value is 
		// implementation-defined."

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

	//! Arithmetic right-shift for integers in two's complement form.
	/*!
	Preconditions:
	n >= 0
	Finite_Integer is an unsigned native integer.

	returns:
	floor(that / 2^n)
	where
	that is interpreted by its two's complement value.
	*/
	template <typename Finite_Integer> 
	requires std::is_unsigned_v<Finite_Integer>
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

	//! Computes floor(that / 2^n).
	/*!
	Preconditions:
	n >= 0
	Finite_Integer is an unsigned native integer.
	*/
	template <typename Finite_Integer> 
	requires std::is_unsigned_v<Finite_Integer>
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

	//! Computes roundTowardsZero(that / 2^n).
	/*!
	Preconditions:
	n >= 0
	Finite_Integer is a signed native integer.
	*/
	template <typename Finite_Integer>
	requires std::is_signed_v<Finite_Integer>
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
