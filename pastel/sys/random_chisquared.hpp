#ifndef PASTELSYS_RANDOM_CHISQUARED_HPP
#define PASTELSYS_RANDOM_CHISQUARED_HPP

#include "pastel/sys/random_chisquared.h"
#include "pastel/sys/random_gamma.h"

namespace Pastel
{

	template <typename Real>
	Real randomChiSquared(integer k)
	{
		PENSURE_OP(k, >, 0);

		return randomGamma<Real>((Real)k / 2, 2);
	}

	template <typename Real>
	Real chiSquaredPdf(
		const NoDeduction<Real>& x,
		integer k)
	{
		PENSURE_OP(k, >, 0);

		return gammaPdf<Real>(x, (Real)k / 2, 2);
	}

}

#endif
