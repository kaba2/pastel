#ifndef PASTELSYS_EXPONENTIAL_BINARY_SEARCH_HPP
#define PASTELSYS_EXPONENTIAL_BINARY_SEARCH_HPP

#include "pastel/sys/exponential_binary_search.h"
#include "pastel/sys/logarithm.h"

namespace Pastel
{

	template <typename Integer, typename Integer_Indicator>
	Integer exponentialBinarySearch(
		Integer minLevel, Integer maxLevel,
		Integer_Indicator indicator)
	{
		// Let L = maxLevel - minLevel.
		integer levels = maxLevel - minLevel;

		// Handle the trivial empty case.
		if (levels == 0)
		{
			return maxLevel;
		}

		// We will now know that L > 0.

		// Let N_k = 2^{2^k - 1}.
		auto N = [&](integer k)
		{
			return (1 << ((1 << k) - 1));
		};

		// k | 2^k - 1 | 2^{2^k - 1}
		// -------------------------
		// 0 |  0      | 1
		// 1 |  1      | 2
		// 2 |  3      | 8
		// 3 |  7      | 128

		// We would like to search the levels using ranges 
		// of the form
		// 
		//     [N_k, N_{k + 1}).
		//
		// On each such range we perform a binary search.
		// How many such ranges do we need to cover all levels?
		//
		// Since L > 0,
		//
		//       N_k <= L
		//    => k <= floor(log2(floor(log2(L)) + 1))
		//
		// By contraposition, if k > floor(log2(floor(log2(L)) + 1)), then
		//
		//    N_k > L.

		// By choosing the number of ranges this way, the upper range
		// may not get covered. We will handle this at the end.
		integer stages = integerLog2(integerLog2(levels) + 1);

		// If L = 16, then stages = 2, and the ranges
		// consist of
		//
		//    [0, 1)
		//    [1, 2)
		//    [2, 8)
		//    [8, 16)

		// If the result of the search is s, how many steps does
		// this algorithm take?
		//
		//    sum_{i = 0}^k log(2^{2^{i + 1} - 1} - 2^{2^i - 1})
		//  = sum_{i = 0}^k [log(2^2^i (2^2^i - 1)) - 1]
		//  = sum_{i = 0}^k [(2^i - 1) + log(2^2^i - 1)]
		// <= sum_{i = 0}^k [2 2^i - 1]
		//  = 2 (2^{k + 1} - 1) - (k + 1)
		//  = 4 2^k - k - 3
		// <= 4 2^k - 3
		// <= 4 (floor(log(s)) + 1) - 3
		//  = 4 floor(log(s)) + 1
		//
		// Therefore, this algorithm has a complexity of
		// O(log(s)), rather than O(log(n)). The price we pay for
		// this improvement is that for large s this algorithm 
		// is about four times slower than the straightforward
		// binary search. On the other, for small s the algorithm
		// is constant time.

		auto indexIndicator = [&](integer index)
		{
			return indicator(minLevel + index);
		};

		integer iMin = 0;
		for (integer k = 0;k <= stages;++k)
		{
			integer iMax = N(k);
			ASSERT_OP(iMax, <=, levels);

			integer i = binarySearch(iMin, iMax, indexIndicator);
			if (i < iMax)
			{
				return minLevel + i;
			}

			iMin = iMax;
			++k;
		}

		if (iMin < levels)
		{
			// Search the rest of the levels.
			integer i = binarySearch(iMin, levels, indexIndicator);
			if (i < levels)
			{
				return minLevel + i;
			}
		}

		// The indicator holds for all elements.
		return maxLevel;
	}

}

#endif
