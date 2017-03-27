// Description: Mutual-information between normal-distribution marginals.

#ifndef PASTELSYS_NORMAL_MUTUAL_INFORMATION_H
#define PASTELSYS_NORMAL_MUTUAL_INFORMATION_H

#include "pastel/sys/random/random_gaussian.h"
#include "pastel/sys/set/set_concept.h"

#include <type_traits>
#include <armadillo>

namespace Pastel
{

	//! Mutual-information between normal-distribution marginals.
	/*!
	partitionSet:
	An increasing sequence 
	*/
	template <
		typename Real,
		typename Set,
		Requires<
			Models<Set, Set_Concept>,
			std::is_convertible<Set_Element<Set>, integer>
		> = 0
	>
	Real mutualInformation(
		const Normal_Distribution<Real>& distribution,
		const Set& partitionSet)
	{
		integer n = distribution.n();

		arma::Mat<Real> covariance = variance(distribution);

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

			result += arma::log_det(
				covariance.submat(*i, *i, *j - 1, *j - 1)).real();

			i = j;
			++j;
		}

		return result / 2;
	}

}

#endif
