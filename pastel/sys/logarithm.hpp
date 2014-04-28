#ifndef PASTELSYS_LOGARITHM_HPP
#define PASTELSYS_LOGARITHM_HPP

#include "pastel/sys/logarithm.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/number_tests.h"
#include "pastel/sys/binary_search.h"

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

	template <typename Finite_Integer>
	integer integerLog2(const Finite_Integer& that)
	{
		ENSURE(positive(that));

		auto zeroShift = [&](integer level)
		{
			return zero(that >> level);
		};

		// We do not use the exponential binary search here:
		// it uses about 2 log(log(that)) comparisons, while the binary
		// search takes only log(bits) comparisons. For bits = 64,
		// and that >= 2^8, the exponential binary search takes
		// >= 6 comparisons, and <= 12 comparisons, while 
		// the binary search always takes 6 comparisons.

		// It is important to get the number of bits correct;
		// right shift by a greater amount of bits is undefined. 
		// In particular, such a right shift may not always be zero.
		// I hit this problem while testing this function with an
		// upper-bound of 64 and infinity<integer>().

		return binarySearch(
			(integer)0, bits(that), 
			zeroShift) - 1;
	}

	template <typename Finite_Integer>
	integer integerCeilLog2(const Finite_Integer& that)
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

