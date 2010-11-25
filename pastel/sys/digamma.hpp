#ifndef PASTEL_DIGAMMA_HPP
#define PASTEL_DIGAMMA_HPP

#include "pastel/sys/digamma.h"

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
