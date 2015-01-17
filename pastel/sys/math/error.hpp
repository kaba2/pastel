#ifndef PASTELSYS_ERROR_HPP
#define PASTELSYS_ERROR_HPP

#include "pastel/sys/math/error.h"

namespace Pastel
{

	template <typename Real>
	Real absoluteError(
		const NoDeduction<Real>& measured,
		const NoDeduction<Real>& correct)
	{
		return mabs(correct - measured);
	}

	template <typename Real>
	Real relativeError(
		const NoDeduction<Real>& measured,
		const NoDeduction<Real>& correct)
	{
		// If correct is zero, then the relative
		// error is infinity.
		return mabs((Real)1 - measured /  correct);
	}

}

#endif
