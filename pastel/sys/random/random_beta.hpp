#ifndef PASTELSYS_RANDOM_BETA_HPP
#define PASTELSYS_RANDOM_BETA_HPP

#include "pastel/sys/random/random_beta.h"
#include "pastel/sys/random/random_gamma.h"
#include "pastel/sys/math_functions.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	Real randomBeta(
		const NoDeduction<Real>& a,
		const NoDeduction<Real>& b)
	{
		PENSURE_OP(a, >, 0);
		PENSURE_OP(b, >, 0);

		Real u = randomGamma<Real>(a);
		
		Real v = 0;
		do
		{
			v = randomGamma<Real>(b);
		}
		while(u + v == 0);

		return u / (u + v);
	}

	template <typename Real>
	Real betaPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& a,
		const NoDeduction<Real>& b)
	{
		if (x < 0 || x > 1)
		{
			return 0;
		}

		return (std::pow(x, a - 1) * std::pow(1 - x, b - 1)) / 
			beta<Real>(a, b);
	}

}

#endif
