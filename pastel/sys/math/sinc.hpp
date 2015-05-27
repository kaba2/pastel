#ifndef PASTELSYS_SINC_HPP
#define PASTELSYS_SINC_HPP

#include "pastel/sys/math/sinc.h"
#include "pastel/sys/math/constants.h"

#include <boost/math/special_functions/sinc.hpp>

namespace Pastel
{

	template <typename Real>
	Real sinc(NoDeduction<Real> x)
	{
		return boost::math::sinc_pi(x * constantPi<Real>());
	}

}

#endif
