#ifndef PASTEL_SYSCOMMON_HPP
#define PASTEL_SYSCOMMON_HPP

#include "pastel/sys/syscommon.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"

#include <sstream>
#include <string>

namespace Pastel
{

	template <typename Type, typename Compare>
	bool equivalent(const Type& left, const Type& right,
		const Compare& compare)
	{
		return !(compare(left, right) || compare(right, left));
	}

	template <typename Type>
	bool equivalent(const Type& left, const Type& right)
	{
		return Pastel::equivalent(left, right, std::less<Type>());
	}

	inline integer toPixelSpanPoint(real t)
	{
		return std::ceil(t - (real)0.5);
	}

	template <int N>
	inline Vector<integer, N> toPixelSpanPoint(
		const Vector<real, N>& that)
	{
		const integer dimension = that.dimension();
		Vector<integer, N> result(ofDimension(dimension));
		for (integer i = 0;i < dimension;++i)
		{
			result[i] = Pastel::toPixelSpanPoint(that[i]);
		}

		return result;
	}

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
