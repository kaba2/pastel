// Description: Two's complement
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_TWOS_COMPLEMENT_H
#define PASTELSYS_TWOS_COMPLEMENT_H

#include "pastel/sys/mytypes.h"

#include <type_traits>

namespace Pastel
{

	//! Two's complement form from a signed integer
	/*!
	Preconditions:
	Finite_Integer is a signed native integer.

	The bit-presentation of a signed integer is implementation-defined,
	and so not portable. However, the bit-representation of an unsigned
	integer is well-defined. This function converts the signed integer,
	no matter what its bit-presentation, portably to its two's complement
	form, and stores it in the bits of the corresponding unsigned integer 
	type.
	*/	
	template <typename Finite_Integer>
	PASTEL_ENABLE_IF(
		std::is_signed<Finite_Integer>, 
		typename std::make_unsigned<Finite_Integer>::type)  
		signedToTwosComplement(const Finite_Integer& that);

	//! Signed integer from a two's complement form.
	/*!
	Preconditions:
	Finite_Integer is an unsigned native integer.

	This is the inverse of signedToTwosComplement(), see its
	documentation.
	*/	
	template <typename Finite_Integer>
	PASTEL_ENABLE_IF(
		std::is_unsigned<Finite_Integer>, 
		typename std::make_signed<Finite_Integer>::type)  
		twosComplementToSigned(const Finite_Integer& that);

}

#include "pastel/sys/twos_complement.hpp"

#endif
