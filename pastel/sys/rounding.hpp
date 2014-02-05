#ifndef PASTELSYS_ROUNDING_HPP
#define PASTELSYS_ROUNDING_HPP

#include "pastel/sys/rounding.h"
#include "pastel/sys/mod.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/number_tests.h"

namespace Pastel
{

	template <typename Integer>
	Integer roundUpTo(
		const Integer& that, const Integer& to)
	{
		PENSURE(!negative(to));

		Integer remainder = mod(that, to);
		if (positive(remainder))
		{
			return (that - remainder) + to;
		}

		return that;
	}

	template <typename Integer>
	Integer roundUpToOdd(const Integer& that)
	{
		return odd(that) ? that : (that + 1);
	}

	template <typename Real>
	PASTEL_ENABLE_IF(std::is_floating_point<Real>, integer)
		roundUpToOdd(const Real& that)
	{
		return Pastel::roundUpToOdd((integer)std::ceil(that));
	}

	template <typename Integer>
	Integer roundUpToEven(const Integer& that)
	{
		return even(that) ? that : (that + 1);
	}

	template <typename Real>
	PASTEL_ENABLE_IF(std::is_floating_point<Real>, integer)
		roundUpToEven(const Real& that)
	{
		return Pastel::roundUpToEven((integer)std::ceil(that));
	}

	inline integer roundUpToPowerOfTwo(integer that)
	{
		PENSURE_OP(that, >=, 0);

		if (that == 0)
		{
			return 1;
		}

		--that;
		for (integer i = 1; i < sizeInBits<integer>();i <<= 1)
		{
			that |= that >> i;
		}

		return that + 1;
	}

	template <typename Integer>
	Integer roundUpToPowerOfTwo(
		const Integer& that, 
		integer power)
	{
		PENSURE(!negative(power));

		Integer remainder = modPowerOfTwo(that, power);
		if (positive(remainder))
		{
			return (that - remainder) + powerOfTwo<Integer>(power);
		}

		return that;
	}

	template <typename Integer>
	Integer divideAndRoundUp(
		const Integer& divide, 
		const Integer& byThis)
	{
		PENSURE(!zero(byThis));

		if ((!negative(divide)) != (!negative(byThis)))
		{
			// If the signs differ, then the integer division
			// computes ceil(a / b), which is what we want.
			return divide / byThis;
		}

		if (!negative(divide))
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
