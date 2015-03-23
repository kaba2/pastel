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
		const ScientificNotation& scientific)
	{
		PASTEL_STATIC_ASSERT(Bits > 0);
		PASTEL_STATIC_ASSERT(E > 0);
		PASTEL_STATIC_ASSERT(M > 0);
		PASTEL_STATIC_ASSERT(1 + E + M <= Bits);

		using Packed = Uint<Bits>;

		// Truncate the mantissa to available bits.
		uint64 scaledMantissa = scientific.mantissa >> (64 - M);

		static PASTEL_CONSTEXPR integer ExponentBias =
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
			((Packed)scientific.negative << (Bits - 1)) +
			((Packed)biasedExponent << M) +
			(Packed)scaledMantissa;

		return packed;
	}

	template <
		typename Float_Ieee,
		EnableIf<std::is_same<Float_Ieee, real32_ieee>>>
	Float_Ieee asIeeeFloat(
		const ScientificNotation& scientific)
	{
		uint32 packed = 
			asIeeeFloatBits<32, 8, 23>(scientific);

		Float_Ieee& result = 
			*(reinterpret_cast<Float_Ieee*>(&packed));

		return result;
	}

	template <
		typename Float_Ieee,
		EnableIf<std::is_same<Float_Ieee, real64_ieee>>>
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
			integer M,
			typename Type, 
			EnableIf<std::is_floating_point<Type>>>
		ScientificNotation ieeeFloatAsScientific(Type that)
		{
			static PASTEL_CONSTEXPR Bits = SizeInBits<Type>::value;

			PASTEL_STATIC_ASSERT(Bits > 0);
			PASTEL_STATIC_ASSERT(E > 0);
			PASTEL_STATIC_ASSERT(M > 0);
			PASTEL_STATIC_ASSERT(1 + E + M == Bits)

			using Packed = Uint<Bits>;
			Packed& packed = *reinterpret_cast<Packed*>(&that);

			Packed mantissa = packed & bitMask<Packed>(0, M);
			Packed biasedExponent = (packed >> M) & bitMask<Packed>(0, E);
			integer bias = ((1 << (E - 1)) - 1);
			integer exponent = biasedExponent - bias;

			return ScientificNotation {negative(that), exponent, mantissa};
		}

	}

	template <
		typename Type,
		EnableIf<std::is_same<Type, real32_ieee>>>
	ScientificNotation asScientific(Type that)
	{
		return IeeeFloat_::ieeeFloatAsScientific<8, 23>(that);
	}

	template <
		typename Type,
		EnableIf<std::is_same<Type, real64_ieee>>>
	ScientificNotation asScientific(Type that)
	{
		return IeeeFloat_::ieeeFloatAsScientific<11, 52>(that);
	}

}

#endif