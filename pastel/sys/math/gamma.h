// Description: Gamma function
// Documentation: math_functions.txt

#ifndef PASTELSYS_GAMMA_H
#define PASTELSYS_GAMMA_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns the gamma function.
	template <typename Real>
	Real gamma(const NoDeduction<Real>& x);

	//! Returns the logarithm of the gamma function.
	template <typename Real>
	Real lnGamma(const NoDeduction<Real>& x);

}

#include "pastel/sys/gamma.hpp"

#endif
