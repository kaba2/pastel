#ifndef PASTELSYS_GAMMA_HPP
#define PASTELSYS_GAMMA_HPP

#include "pastel/sys/math/gamma.h"

#include <boost/math/special_functions/gamma.hpp>

namespace Pastel
{

	template <typename Real>
	Real gamma(const NoDeduction<Real>& x)
	{
		return boost::math::tgamma(x);
	}

	template <typename Real>
	Real lnGamma(const NoDeduction<Real>& x)
	{
		return boost::math::lgamma(x);
	}

}

#endif
