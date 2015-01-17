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
		const NoDeduction<Real>& measured,
		const NoDeduction<Real>& correct);

	//! Returns the relative error of a measured value to a correct value.
	template <typename Real>
	Real relativeError(
		const NoDeduction<Real>& measured,
		const NoDeduction<Real>& correct);

}

#include "pastel/sys/math/error.hpp"

#endif
