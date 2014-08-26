#ifndef PASTELSYS_ERROR_HPP
#define PASTELSYS_ERROR_HPP

#include "pastel/sys/error.h"

namespace Pastel
{

	template <typename Real>
	Real absoluteError(
		const PASTEL_NO_DEDUCTION(Real)& measured,
		const PASTEL_NO_DEDUCTION(Real)& correct)
	{
		return mabs(correct - measured);
	}

	template <typename Real>
	Real relativeError(
		const PASTEL_NO_DEDUCTION(Real)& measured,
		const PASTEL_NO_DEDUCTION(Real)& correct)
	{
		PENSURE(!zero(correct));
		return mabs((Real)1 - measured /  correct);
	}

}

#endif
