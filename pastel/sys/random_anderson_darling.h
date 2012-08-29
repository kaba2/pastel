// Description: Anderson-Darling statistic
// Documentation: random_goodness.txt

#ifndef PASTELSYS_RANDOM_ANDERSON_DARLING_H
#define PASTELSYS_RANDOM_ANDERSON_DARLING_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Anderson-Darling statistic
	template <typename Real, typename Real_ConstRange>
	Real gaussianAndersonDarling(
		const Real_ConstRange& input,
		PASTEL_NO_DEDUCTION(Real) mean = nan<Real>(),
		PASTEL_NO_DEDUCTION(Real) deviation = nan<Real>());

}

#include "pastel/sys/random_anderson_darling.hpp"

#endif
