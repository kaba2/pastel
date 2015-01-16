// Description: Digamma function
// Documentation: math_functions.txt

#ifndef PASTELSYS_DIGAMMA_H
#define PASTELSYS_DIGAMMA_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Digamma function
	template <typename Real>
	Real digamma(const NoDeduction<Real>& x);

}

#include "pastel/sys/digamma.hpp"

#endif
