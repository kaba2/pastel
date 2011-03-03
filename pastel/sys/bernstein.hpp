#ifndef PASTEL_BERNSTEIN_HPP
#define PASTEL_BERNSTEIN_HPP

#include "pastel/sys/bernstein.h"
#include "pastel/sys/ensure.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	Real bernstein(integer n, integer i, const Real& t)
	{
		PENSURE_OP(n, >=, 0);
		PENSURE_OP(i, >=, 0);
		PENSURE_OP(i, <=, n);

		// bernstein(n,i,t) = choose(n,i) *
		//     pow(1 - t, n - i) * pow(t, i)

		return choose<Real>(n, i) *
			std::pow(1 - t, Real(n-i)) *
			std::pow(t, Real(i));
	}

}

#endif
