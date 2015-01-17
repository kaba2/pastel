#ifndef PASTELSYS_DIGAMMA_HPP
#define PASTELSYS_DIGAMMA_HPP

#include "pastel/sys/math/digamma.h"

#include <boost/math/special_functions/digamma.hpp>

namespace Pastel
{

	template <typename Real>
	Real digamma(const NoDeduction<Real>& x)
	{
		return boost::math::digamma(x);
	}

}

#endif
