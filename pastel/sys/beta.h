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
		PASTEL_NO_DEDUCTION(Real) x,
		PASTEL_NO_DEDUCTION(Real) y);

	//! [Logarithm of the beta function]
	template <typename Real>
	Real lnBeta(
		PASTEL_NO_DEDUCTION(Real) x,
		PASTEL_NO_DEDUCTION(Real) y);

}

#include "pastel/sys/beta.hpp"

#endif
