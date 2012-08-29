#ifndef PASTELSYS_DIGAMMA_HPP
#define PASTELSYS_DIGAMMA_HPP

#include "pastel/sys/digamma.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real>
	Real digamma(integer n)
	{
		PENSURE_OP(n, >, 0);

		return Pastel::digammaReal64(n);
	}

}

#endif
