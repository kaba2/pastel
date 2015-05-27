// Description: Sinc function
// Documentation: math_functions.txt

#ifndef PASTELSYS_SINC_H
#define PASTELSYS_SINC_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Computes the cardinal sine.
	/*!
	sinc(x) = sin(pi * x) / (pi * x).
	*/
	template <typename Real>
	Real sinc(NoDeduction<Real> x);

}

#include "pastel/sys/math/sinc.hpp"

#endif
