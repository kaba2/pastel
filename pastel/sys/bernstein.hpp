#ifndef PASTELSYS_BERNSTEIN_HPP
#define PASTELSYS_BERNSTEIN_HPP

#include "pastel/sys/bernstein.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/binomial.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	Real bernstein(integer i, integer n, const Real& t)
	{
		PENSURE_OP(n, >=, 0);
		PENSURE_OP(i, >=, 0);
		PENSURE_OP(i, <=, n);

		// bernstein(i, n, t) = binomial(i, n) *
		//     pow(1 - t, n - i) * pow(t, i)

		return binomial<Real>(i, n) *
			std::pow(1 - t, Real(n-i)) *
			std::pow(t, Real(i));
	}

}

#endif
