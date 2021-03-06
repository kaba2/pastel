// Description: Kolmogorov-Smirnov statistic
// Documentation: random_goodness.txt

#ifndef PASTELSYS_RANDOM_KOLMOGOROV_SMIRNOV_H
#define PASTELSYS_RANDOM_KOLMOGOROV_SMIRNOV_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Anderson-Darling statistic
	template <typename Real, typename Real_ConstRange>
	Real gaussianKolmogorovSmirnov(
		const Real_ConstRange& input,
		NoDeduction<Real> mean = (Real)Nan(),
		NoDeduction<Real> deviation = (Real)Nan());

}

#include "pastel/sys/random/random_kolmogorov_smirnov.hpp"

#endif
