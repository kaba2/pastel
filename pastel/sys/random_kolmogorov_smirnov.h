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
		NoDeduction<Real> mean = nan<Real>(),
		NoDeduction<Real> deviation = nan<Real>());

}

#include "pastel/sys/random_kolmogorov_smirnov.hpp"

#endif
