#ifndef PASTELSYS_SCALE_INTEGER_HPP
#define PASTELSYS_SCALE_INTEGER_HPP

#include "pastel/sys/scale_integer.h"

namespace Pastel
{

	template <
		int FromBits, 
		int ToBits, 
		typename Integer,
		EnableIfC<(ToBits < FromBits)>*>
	Integer scaleInteger(const Integer& number)
	{
		PASTEL_STATIC_ASSERT(FromBits > 0);
		PASTEL_STATIC_ASSERT(ToBits > 0);

		static PASTEL_CONSTEXPR int DeltaBits = FromBits - ToBits;
		PASTEL_STATIC_ASSERT(DeltaBits >= 0);

		return (number >> DeltaBits);
	}

	template <
		int FromBits, 
		int ToBits, 
		typename Integer,
		EnableIfC<(ToBits == FromBits)>*>
	Integer scaleInteger(const Integer& number)
	{
		return number;
	}

	template <
		int FromBits, 
		int ToBits, 
		typename Integer,
		EnableIfC<(ToBits > FromBits && ToBits <= 2 * FromBits)>*>
	Integer scaleInteger(const Integer& number)
	{
		PASTEL_STATIC_ASSERT(FromBits > 0);
		PASTEL_STATIC_ASSERT(ToBits > 0);

		static PASTEL_CONSTEXPR int DeltaBits = ToBits - FromBits;
		PASTEL_STATIC_ASSERT(DeltaBits >= 0);

		return (number << DeltaBits) +
			(number >> (FromBits - DeltaBits));
	}

	template <
		int FromBits, 
		int ToBits, 
		typename Integer,
		EnableIfC<(ToBits > 2 * FromBits && ToBits <= 3 * FromBits)>*>
	Integer scaleInteger(const Integer& number)
	{
		PASTEL_STATIC_ASSERT(FromBits > 0);
		PASTEL_STATIC_ASSERT(ToBits > 0);

		static PASTEL_CONSTEXPR int DeltaBits = ToBits - FromBits;
		PASTEL_STATIC_ASSERT(DeltaBits >= 0);

		return (number << DeltaBits) +
			(number << (DeltaBits - FromBits)) +
			(number >> (2 * FromBits - DeltaBits));
	}

	template <
		int FromBits, 
		int ToBits, 
		typename Integer,
		EnableIfC<(ToBits > 3 * FromBits && ToBits <= 4 * FromBits)>*>
	Integer scaleInteger(const Integer& number)
	{
		PASTEL_STATIC_ASSERT(FromBits > 0);
		PASTEL_STATIC_ASSERT(ToBits > 0);

		static PASTEL_CONSTEXPR int DeltaBits = ToBits - FromBits;
		PASTEL_STATIC_ASSERT(DeltaBits >= 0);

		return (number << DeltaBits) +
			(number << (DeltaBits - FromBits)) +
			(number << (DeltaBits - 2 * FromBits)) +
			(number >> (3 * FromBits - DeltaBits));
	}

}

#endif
