// Description: Absolute and relative error
// Documentation: math_functions.txt

#ifndef PASTELSYS_ERROR_H
#define PASTELSYS_ERROR_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns the absolute error of a measured value to a correct value.
	template <typename Real>
	Real absoluteError(
		const PASTEL_NO_DEDUCTION(Real)& measured,
		const PASTEL_NO_DEDUCTION(Real)& correct);

	//! Returns the relative error of a measured value to a correct value.
	template <typename Real>
	Real relativeError(
		const PASTEL_NO_DEDUCTION(Real)& measured,
		const PASTEL_NO_DEDUCTION(Real)& correct);

}

#include "pastel/sys/error.hpp"

#endif
