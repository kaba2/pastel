// Description: Two's complement

#ifndef PASTELSYS_TWOS_COMPLEMENT_H
#define PASTELSYS_TWOS_COMPLEMENT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/integer/finite_integer_concept.h"
#include "pastel/sys/integer/integer_concept.h"

#include <type_traits>

namespace Pastel
{

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
	template <typename Integer, EnableIf<std::is_signed<Integer>> = 0>
	typename std::make_unsigned<Integer>::type 
	signedToTwosComplement(const Integer& that);

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
	template <typename Finite_Integer, EnableIf<std::is_unsigned<Finite_Integer>> = 0>
	typename std::make_signed<Finite_Integer>::type
		twosComplementToSigned(const Finite_Integer& that);

	//! Returns whether 'that' represents a negative value.
	/*!
	A two's complement integer is negative if and only if
	its most-significant bit is 1.
	*/
	template <typename Finite_Integer, EnableIf<std::is_unsigned<Finite_Integer>> = 0>
	bool twosComplementNegative(const Finite_Integer& that);

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
	template <typename Finite_Integer, EnableIf<std::is_unsigned<Finite_Integer>> = 0>
	Finite_Integer arithmeticShiftRight(
			const Finite_Integer& that, 
			integer n);

	//! Computes floor(that / 2^n).
	/*!
	Preconditions:
	n >= 0
	Finite_Integer is an unsigned native integer.
	*/
	template <typename Finite_Integer, EnableIf<std::is_unsigned<Finite_Integer>> = 0>
	Finite_Integer shiftRight(
			const Finite_Integer& that, 
			integer n);

	//! Computes roundTowardsZero(that / 2^n).
	/*!
	Preconditions:
	n >= 0
	Finite_Integer is a signed native integer.
	*/
	template <typename Finite_Integer, EnableIf<std::is_signed<Finite_Integer>> = 0>
	Finite_Integer shiftRight(
			const Finite_Integer& that, 
			integer n);

}

#include "pastel/sys/bit/twos_complement.hpp"

#endif
