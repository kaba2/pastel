#ifndef PASTELSYS_BETA_HPP
#define PASTELSYS_BETA_HPP

#include "pastel/sys/beta.h"
#include "pastel/sys/gamma.h"

namespace Pastel
{

	template <typename Real>
	Real beta(
		NoDeduction<Real> x,
		NoDeduction<Real> y)
	{
		return (gamma<Real>(x) * gamma<Real>(y)) / 
			gamma<Real>(x + y);
	}

	template <typename Real>
	Real lnBeta(
		NoDeduction<Real> x,
		NoDeduction<Real> y)
	{
		return (lnGamma<Real>(x) + lnGamma<Real>(y)) - 
			lnGamma<Real>(x + y);
	}

}

#endif
