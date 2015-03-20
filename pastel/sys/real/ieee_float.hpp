#ifndef PASTELSYS_IEEE_FLOAT_HPP
#define PASTELSYS_IEEE_FLOAT_HPP

#include "pastel/sys/real/ieee_float.h"

namespace Pastel
{

	template <
		integer Bits,
		integer E,
		integer M>
	Uint<Bits> asIeeeFloatBits(
		bool negative, 
		integer exponent, 
		uint64 scaledMantissa)
	{
		PASTEL_STATIC_ASSERT(Bits > 0);
		PASTEL_STATIC_ASSERT(E > 0);
		PASTEL_STATIC_ASSERT(M > 0);
		PASTEL_STATIC_ASSERT(1 + E + M <= Bits);

		using Packed = Uint<Bits>;

		// Truncate the mantissa to available bits.
		scaledMantissa >>= 64 - M;

		static PASTEL_CONSTEXPR integer ExponentBias =
			(1 << (E - 1)) - 1;

		integer biasedExponent = 
			exponent + ExponentBias;

		if (biasedExponent < 0)
		{
			// Underflow (==> +/- 0).
			return (Packed)negative << (Bits - 1);
		}

		// Nothing special needs to be done for sub-normal
		// numbers (biasedExponent == 0).

		static PASTEL_CONSTEXPR integer maxBiasedExponent =
			(1 << E) - 1;
		
		if (biasedExponent >= maxBiasedExponent)
		{
			// Set to infinity or -infinity.
			biasedExponent = maxBiasedExponent;
			// We do not allow to form NaNs.
			scaledMantissa = 0;
		}

		Packed packed = 
			((Packed)negative << (Bits - 1)) +
			((Packed)biasedExponent << M) +
			(Packed)scaledMantissa;

		return packed;
	}

	template <
		typename Float_Ieee,
		EnableIf<std::is_same<Float_Ieee, real32_ieee>>>
	Float_Ieee asIeeeFloat(
		bool negative, 
		integer exponent, 
		uint64 scaledMantissa)
	{
		uint32 packed = 
			asIeeeFloatBits<32, 8, 23>(negative, exponent, scaledMantissa);

		Float_Ieee& result = 
			*(reinterpret_cast<Float_Ieee*>(&packed));

		return result;
	}

	template <
		typename Float_Ieee,
		EnableIf<std::is_same<Float_Ieee, real64_ieee>>>
	Float_Ieee asIeeeFloat(
		bool negative, 
		integer exponent, 
		uint64 scaledMantissa)
	{
		uint64 packed = 
			asIeeeFloatBits<64, 11, 52>(negative, exponent, scaledMantissa);

		Float_Ieee& result = 
			*(reinterpret_cast<Float_Ieee*>(&packed));

		return result;
	}

}

#endif
