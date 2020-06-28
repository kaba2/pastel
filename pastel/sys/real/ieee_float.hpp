#ifndef PASTELSYS_IEEE_FLOAT_HPP
#define PASTELSYS_IEEE_FLOAT_HPP

#include "pastel/sys/real/ieee_float.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/real/scientific_notation_fwd.h"
#include "pastel/sys/bit/bitmask.h"

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
		int M>
	UIntegerOfSize<Bits> asIeeeFloatBits(
		const ScientificNotation& scientific)
	{
		PASTEL_STATIC_ASSERT(Bits > 0);
		PASTEL_STATIC_ASSERT(E > 0);
		PASTEL_STATIC_ASSERT(M > 0);
		PASTEL_STATIC_ASSERT(1 + E + M <= Bits);

		using Packed = UIntegerOfSize<Bits>;

		// Truncate the mantissa to available bits.
		uint64 scaledMantissa = scientific.mantissa >> (64 - M);

		static constexpr integer ExponentBias =
			(1 << (E - 1)) - 1;

		integer biasedExponent = 
			scientific.exponent + ExponentBias;

		if (biasedExponent < 0)
		{
			// Underflow (==> +/- 0).
			return (Packed)scientific.negative << (Bits - 1);
		}

		// Nothing special needs to be done for sub-normal
		// numbers (biasedExponent == 0).

		static constexpr integer maxBiasedExponent =
			(1 << E) - 1;
		
		if (biasedExponent >= maxBiasedExponent)
		{
			// Set to infinity or -infinity.
			biasedExponent = maxBiasedExponent;
			// We do not allow to form NaNs.
			scaledMantissa = 0;
		}

		Packed packed = 
			((Packed)scientific.negative << (Bits - 1)) +
			((Packed)biasedExponent << M) +
			(Packed)scaledMantissa;

		return packed;
	}

	//! Returns the closest 32-bit IEEE floating-point number.
	/*!
	See the documentation for ieeeFloatBits().
	*/
	template <
		typename Float_Ieee,
		Requires<std::is_same<Float_Ieee, real32_ieee>> = 0>
	Float_Ieee asIeeeFloat(
		const ScientificNotation& scientific)
	{
		uint32 packed = 
			asIeeeFloatBits<32, 8, 23>(scientific);

		Float_Ieee& result = 
			*(reinterpret_cast<Float_Ieee*>(&packed));

		return result;
	}

	//! Returns the closest 64-bit IEEE floating-point number.
	/*!
	See the documentation for ieeeFloatBits().
	*/
	template <
		typename Float_Ieee,
		Requires<std::is_same<Float_Ieee, real64_ieee>> = 0>
	Float_Ieee asIeeeFloat(const ScientificNotation& scientific)
	{
		uint64 packed = 
			asIeeeFloatBits<64, 11, 52>(scientific);

		Float_Ieee& result = 
			*(reinterpret_cast<Float_Ieee*>(&packed));

		return result;
	}

	namespace IeeeFloat_
	{

		template <
			integer E,
			int M,
			typename Type, 
			Requires<std::is_floating_point<Type>> = 0>
		ScientificNotation ieeeFloatAsScientific(Type that)
		{
			static constexpr integer Bits = SizeInBits<Type>::value;

			PASTEL_STATIC_ASSERT(Bits > 0);
			PASTEL_STATIC_ASSERT(E > 0);
			PASTEL_STATIC_ASSERT(M > 0);
			PASTEL_STATIC_ASSERT(1 + E + M == Bits)

			using Packed = UIntegerOfSize<Bits>;
			Packed& packed = *reinterpret_cast<Packed*>(&that);

			Packed packedMantissa = packed & bitMask<Packed>(0, M);
			Packed biasedExponent = (packed >> M) & bitMask<Packed>(0, E);
			integer bias = ((1 << (E - 1)) - 1);
			integer exponent = biasedExponent - bias;
			uint64 mantissa = (uint64)packedMantissa << (64 - M);

			return ScientificNotation {negative(that), exponent, mantissa};
		}

	}

	//! Returns a 32-bit IEEE floating-point number in scientific notation.
	template <
		typename Type,
		Requires<std::is_same<Type, real32_ieee>> = 0>
	ScientificNotation ieeeFloatAsScientific(Type that)
	{
		return IeeeFloat_::ieeeFloatAsScientific<8, 23>(that);
	}

	//! Returns a 64-bit IEEE floating-point number in scientific notation.
	template <
		typename Type,
		Requires<std::is_same<Type, real64_ieee>> = 0>
	ScientificNotation ieeeFloatAsScientific(Type that)
	{
		return IeeeFloat_::ieeeFloatAsScientific<11, 52>(that);
	}

}

#endif
