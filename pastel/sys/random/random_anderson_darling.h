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
		NoDeduction<Real> mean = (Real)Nan(),
		NoDeduction<Real> deviation = (Real)Nan());

}

#include "pastel/sys/random/random_anderson_darling.hpp"

#endif
