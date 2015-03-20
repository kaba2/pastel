// Description: IEEE floating point numbers

#ifndef PASTELSYS_IEEE_FLOAT_H
#define PASTELSYS_IEEE_FLOAT_H

#include "pastel/sys/mytypes.h"

#include <type_traits>

namespace Pastel
{

	//! Returns the bits of the closest IEEE floating-point number.
	/*!
	Preconditions:
	Bits > 0
	E > 0
	M > 0
	E + M <= Bits

	returns:
	The closest floating-point number representative for
	(-1)^negative * 2^exponent * (1 + scaledMantissa / 2^64).
	The result will never be a not-a-number (NaN). Overflows
	return (-1)^negative infinity, and underflows return 
	(-1)^negative 0.
	*/
	template <
		integer Bits,
		integer E,
		integer M>
	Uint<Bits> asIeeeFloatBits(
		bool negative, 
		integer exponent, 
		uint64 scaledMantissa);

	//! Returns the closest 32-bit IEEE floating-point number.
	/*!
	See the documentation for ieeeFloatBits().
	*/
	template <
		typename Float_Ieee,
		EnableIf<std::is_same<Float_Ieee, real32_ieee>> = 0>
	Float_Ieee asIeeeFloat(
		bool negative, 
		integer exponent, 
		uint64 scaledMantissa);

	//! Returns the closest 64-bit IEEE floating-point number.
	/*!
	See the documentation for ieeeFloatBits().
	*/
	template <
		typename Float_Ieee,
		EnableIf<std::is_same<Float_Ieee, real64_ieee>> = 0>
	Float_Ieee asIeeeFloat(
		bool negative, 
		integer exponent, 
		uint64 scaledMantissa);

}

#include "pastel/sys/real/ieee_float.hpp"

#endif
