#ifndef PASTEL_ROUNDING_HPP
#define PASTEL_ROUNDING_HPP

#include "pastel/sys/rounding.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	inline integer roundUpTo(integer that, integer to)
	{
		PENSURE_OP(to, >=, 0);

		const integer remainder = mod(that, to);
		if (remainder > 0)
		{
			return that + (to - remainder);
		}

		return that;
	}

	inline integer roundUpToOdd(integer that)
	{
		if (odd(that))
		{
			return that;
		}

		return that + 1;
	}

	inline integer roundUpToOdd(real that)
	{
		return Pastel::roundUpToOdd((integer)std::ceil(that));
	}

	inline integer roundUpToEven(integer that)
	{
		if (odd(that))
		{
			return that + 1;
		}

		return that;
	}

	inline integer roundUpToEven(real that)
	{
		return Pastel::roundUpToEven((integer)std::ceil(that));
	}

	inline integer roundUpToPowerOf2(integer that, integer power)
	{
		PENSURE_OP(that, >=, 0);
		PENSURE_OP(power, >=, 0);

		const integer to = (1 << power);
		const integer remainder = that & (to - 1);

		if (remainder > 0)
		{
			return that + (to - remainder);
		}

		return that;
	}

	inline integer roundUpToPowerOf2(integer that)
	{
		PASTEL_STATIC_ASSERT(sizeof(integer) == sizeof(int32));

		PENSURE_OP(that, >=, 0);

		// NOTE: Works only for 32 bit integer.

		--that;

		that |= that >> 1;
		that |= that >> 2;
		that |= that >> 4;
		that |= that >> 8;
		that |= that >> 16;

		return that + 1;
	}

}

#endif
