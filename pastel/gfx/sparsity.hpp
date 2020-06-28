#ifndef PASTELGFX_SPARSITY_HPP
#define PASTELGFX_SPARSITY_HPP

#include "pastel/gfx/sparsity.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real_Range>
	ranges::range_value_t<Real_Range>
		sparsity2(const Real_Range& input)
	{
		typedef ranges::range_value_t<Real_Range>
			Real;

		// This is the squared 2-sparsity measure.
		// See 'sparsity.txt'.

		Real mean = 0;
		Real squareMean = 0;
		integer n = 0;

		// Compute mean and square mean.
		{
			Real_Iterator iter = input.begin();
			Real_Iterator end = input.end();
			while(iter != end)
			{

				const Real value = *iter;

				mean += value;
				squareMean += square(value);
				++n;

				++iter;
			}
		}

		ENSURE_OP(n, >=, 2);

		Real variance = 0;

		// Compute variance.
		{
			Real_Iterator iter = input.begin();
			Real_Iterator end = input.end();
			while(iter != end)
			{

				const Real value = *iter;
				variance += square(value - mean);
				++iter;
			}
		}
		
		return ((Real)n / (n - 1)) * (variance / squareMean);
	}

}

#endif
