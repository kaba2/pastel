// Description: Mutual-information between normal-distribution marginals.

#ifndef PASTELSYS_NORMAL_MUTUAL_INFORMATION_H
#define PASTELSYS_NORMAL_MUTUAL_INFORMATION_H

#include "pastel/sys/random/random_gaussian.h"
#include "pastel/sys/range/range_concept.h"

#include <type_traits>
#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Mutual-information between normal-distribution marginals.
	/*!
	partitionSet:
	An increasing sequence 
	*/
	template <
		typename Real,
		Range_Concept Set
	>
	requires std::is_convertible_v<Range_Value<Set>, integer>
	Real mutualInformation(
		const Normal_Distribution<Real>& distribution,
		const Set& partitionSet)
	{
		integer n = distribution.n();

		Matrix<Real> covariance = variance(distribution);

		Real result = -std::log(distribution.detCovariance());
		
		auto i = begin(partitionSet);
		auto j = std::next(i);
		while (j != end(partitionSet))
		{
			ENSURE_OP(*i, >=, 0);
			ENSURE_OP(*i, <=, *j);
			ENSURE_OP(*j, <=, n);

			if (*i == *j)
			{
				// Zero-dimensional marginal;
				// skip over it.
				continue;
			}

			int n = *j - *i;

			result += std::log(determinant(
				covariance.block(*i, *i, n, n)));

			i = j;
			++j;
		}

		return result / 2;
	}

}

#endif
