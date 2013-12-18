#ifndef PASTELSYS_LOGARITHM_HPP
#define PASTELSYS_LOGARITHM_HPP

#include "pastel/sys/logarithm.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/number_tests.h"
#include "pastel/sys/integer_integer.h"
#include "pastel/sys/exponential_binary_search.h"

#include <cmath>
#include <iostream>

namespace Pastel
{

	template <typename Real>
	integer floorLog2(
		PASTEL_NO_DEDUCTION(Real) x)
	{
		PENSURE_OP(x, >=, 1);

		integer power = 0;
		while (x >= 2)
		{
			x /= 2;
			++power;
		}

		return power;
	}

	template <typename Real>
	Real log2(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		PENSURE_OP(x, >=, 0);

		return std::log(x) / constantLn2<Real>();
	}

	inline integer integerLog2(integer that)
	{
		ENSURE_OP(that, >, 0);

		auto nonZeroShift = [&](integer level)
		{
			return (that >> level) != 0;
		};

		// It is important to get the number of bits correct;
		// right shift by a greater amount of bits is undefined. 
		// In particular, such a right shift may not always be zero.
		// I hit this problem while testing this function with an
		// upper-bound of 64 and infinity<integer>().

		// PORTABILITY: Assumes bytes have 8 bits.
		integer bits = sizeof(integer) * 8;
		return exponentialBinarySearch(
			(integer)0, bits, 
			nonZeroShift) - 1;
	}

	inline integer integerCeilLog2(integer that)
	{
		ENSURE_OP(that, >, 0);
		if (isPowerOfTwo(that))
		{
			return integerLog2(that);
		}

		return integerLog2(that) + 1;
	}

}

#endif

