// Description: Beta function
// Documentation: math_functions.txt

#ifndef PASTELSYS_BETA_H
#define PASTELSYS_BETA_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! [Beta function]
	template <typename Real>
	Real beta(
		NoDeduction<Real> x,
		NoDeduction<Real> y);

	//! [Logarithm of the beta function]
	template <typename Real>
	Real lnBeta(
		NoDeduction<Real> x,
		NoDeduction<Real> y);

}

#include "pastel/sys/math/beta.hpp"

#endif
