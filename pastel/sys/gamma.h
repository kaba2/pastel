// Description: Gamma function
// Documentation: math_functions.txt

#ifndef PASTEL_GAMMA_H
#define PASTEL_GAMMA_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! [Gamma function]
	template <typename Real>
	Real gamma(PASTEL_NO_DEDUCTION(Real) z);

	//! [Logarithm of the gamma function]
	template <typename Real>
	Real lnGamma(PASTEL_NO_DEDUCTION(Real) z);

}

#include "pastel/sys/gamma.hpp"

#endif
