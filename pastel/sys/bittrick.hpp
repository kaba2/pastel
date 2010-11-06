#ifndef PASTEL_BITTRICK_HPP
#define PASTEL_BITTRICK_HPP

#include "pastel/sys/bittrick.h"

namespace Pastel
{

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits < FromBits), Integer>::type
		scaleInteger(const Integer& number)
	{
		PASTEL_STATIC_ASSERT(FromBits > 0);
		PASTEL_STATIC_ASSERT(ToBits > 0);

		enum
		{
			DeltaBits = FromBits - ToBits
		};

		return (number >> DeltaBits);
	}

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits == FromBits), Integer>::type
		scaleInteger(const Integer& number)
	{
		return number;
	}

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits > FromBits && ToBits <= 2 * FromBits), Integer>::type
		scaleInteger(const Integer& number)
	{
		PASTEL_STATIC_ASSERT(FromBits > 0);
		PASTEL_STATIC_ASSERT(ToBits > 0);

		enum
		{
			DeltaBits = ToBits - FromBits
		};

		PASTEL_STATIC_ASSERT(DeltaBits >= 0);

		return (number << DeltaBits) +
			(number >> (FromBits - DeltaBits));
	}

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits > 2 * FromBits && ToBits <= 3 * FromBits), Integer>::type
		scaleInteger(const Integer& number)
	{
		PASTEL_STATIC_ASSERT(FromBits > 0);
		PASTEL_STATIC_ASSERT(ToBits > 0);

		enum
		{
			DeltaBits = ToBits - FromBits
		};

		PASTEL_STATIC_ASSERT(DeltaBits >= 0);

		return (number << DeltaBits) +
			(number << (DeltaBits - FromBits)) +
			(number >> (2 * FromBits - DeltaBits));
	}

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits > 3 * FromBits && ToBits <= 4 * FromBits), Integer>::type
		scaleInteger(const Integer& number)
	{
		PASTEL_STATIC_ASSERT(FromBits > 0);
		PASTEL_STATIC_ASSERT(ToBits > 0);

		enum
		{
			DeltaBits = ToBits - FromBits
		};

		PASTEL_STATIC_ASSERT(DeltaBits >= 0);

		return (number << DeltaBits) +
			(number << (DeltaBits - FromBits)) +
			(number << (DeltaBits - 2 * FromBits)) +
			(number >> (3 * FromBits - DeltaBits)) +;
	}

	template <int FromBit, int Bits, typename Integer>
	Integer extractBits(const Integer& number)
	{
		PASTEL_STATIC_ASSERT(FromBit >= 0);
		PASTEL_STATIC_ASSERT(Bits > 0);

		return (number >> FromBit) & ((1 << Bits) - 1);
	}

	template <typename Integer>
	Integer extractBits(const Integer& number, 
		integer fromBit, integer bits)
	{
		PENSURE_OP(fromBit, >=, 0);
		PENSURE_OP(bits, >, 0);

		return (number >> fromBit) & ((1 << bits) - 1);
	}

}

#endif
