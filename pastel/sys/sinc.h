// Description: Sinc function
// Documentation: math_functions.txt

#ifndef PASTEL_SINC_H
#define PASTEL_SINC_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Computes the [cardinal sine].
	/*!
	sinc(x) = sin(pi * x) / (pi * x).
	*/
	template <typename Real>
	Real sinc(const PASTEL_NO_DEDUCTION(Real)& x);

}

#include "pastel/sys/sinc.hpp"

#endif
