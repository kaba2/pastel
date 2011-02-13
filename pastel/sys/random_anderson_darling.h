// Description: Anderson-Darling statistic
// Documentation: random_goodness.txt

#ifndef PASTEL_RANDOM_ANDERSON_DARLING_H
#define PASTEL_RANDOM_ANDERSON_DARLING_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/forwarditerator_range.h"

namespace Pastel
{

	//! Anderson-Darling statistic
	template <typename Real, typename Real_ConstIterator>
	Real gaussianAndersonDarling(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		PASTEL_NO_DEDUCTION(Real) mean = nan<Real>(),
		PASTEL_NO_DEDUCTION(Real) deviation = nan<Real>());

}

#include "pastel/sys/random_anderson_darling.hpp"

#endif
