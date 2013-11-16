#ifndef PASTELSYS_ROUNDING_HPP
#define PASTELSYS_ROUNDING_HPP

#include "pastel/sys/rounding.h"
#include "pastel/sys/mod.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/number_tests.h"

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

	inline integer roundUpToPowerOf2(integer that)
	{
		PENSURE_OP(that, >=, 0);

		if (that == 0)
		{
			return 1;
		}

		--that;
		for (integer i = 1; i < sizeof(integer) * 8;i <<= 1)
		{
			that |= that >> i;
		}

		return that + 1;
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

	template <typename Integer>
	Integer divideAndRoundUp(const Integer& divide, const Integer& byThis)
	{
		PENSURE_OP(byThis, !=, 0);

		if ((divide >= 0) ^ (byThis >= 0))
		{
			// If the signs differ, then the integer division
			// computes ceil(a / b), which is what we want.
			return divide / byThis;
		}

		if (divide >= 0)
		{
			// Both are non-negative, and the integer division
			// computes floor(a / b). We use the formula
			// ceil(a / b) = floor((a + (b - 1)) / b).
			return (divide + byThis - 1) / byThis;
		}

		// Both are negative. Turn them positive,
		// and do as above:
		//
		// ceil(a / b) = ceil((-a) / (-b))
		//             = floor((-a + ((-b) - 1)) / (-b))
		//             = floor((-a - b - 1) / (-b))
		//             = floor(-(a + b + 1) / (-b))
		//             = floor((a + b + 1) / b)

		return (divide + byThis + 1) / byThis;
	}

}

#endif
