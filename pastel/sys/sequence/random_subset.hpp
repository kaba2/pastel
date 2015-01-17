#ifndef PASTELSYS_RANDOM_SUBSET_HPP
#define PASTELSYS_RANDOM_SUBSET_HPP

#include "pastel/sys/sequence/random_subset.h"
#include "pastel/sys/random/random_uniform.h"

#include <utility>

namespace Pastel
{

	template <typename RandomAccess_Iterator>
	void randomSubset(
		RandomAccess_Iterator begin,
		RandomAccess_Iterator end,
		integer k)
	{
		integer n = end - begin;

		PENSURE_OP(k, >=, 0);
		PENSURE_OP(k, <=, n);
		
		if (k == 0 || begin == end)
		{
			return;
		}

		// By the following branching
		// we guarantee that at most n / 2
		// swaps are needed.

		if (2 * k < n)
		{
			// Move to the front those elements which
			// will be in the subset.
			for (integer i = 0;i < k;++i)
			{
				integer index = randomInteger(n - i);

				using std::swap;

				swap(*begin, *(begin + index));

				++begin;
			}
		}
		else
		{
			// Move to the back those elements which
			// will _not_ be in the subset.
			for (integer i = 0;i < n - k;++i)
			{
				integer index = randomInteger(n - i);

				--end;

				using std::swap;

				swap(*end, *(begin + index));
			}
		}
	}

}

#endif
