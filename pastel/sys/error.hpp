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
		return mabs(measured - correct);
	}

	template <typename Real>
	Real relativeError(
		const PASTEL_NO_DEDUCTION(Real)& measured,
		const PASTEL_NO_DEDUCTION(Real)& correct)
	{
		return absoluteError<Real>(measured, correct) / correct;
	}

}

#endif
