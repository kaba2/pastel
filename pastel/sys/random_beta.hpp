#ifndef PASTEL_RANDOM_BETA_HPP
#define PASTEL_RANDOM_BETA_HPP

#include "pastel/sys/random_beta.h"
#include "pastel/sys/random_gamma.h"
#include "pastel/sys/math_functions.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	Real randomBeta(
		const PASTEL_NO_DEDUCTION(Real)& a,
		const PASTEL_NO_DEDUCTION(Real)& b)
	{
		PENSURE_OP(a, >, 0);
		PENSURE_OP(b, >, 0);

		const Real u = randomGamma<Real>(a);
		const Real v = randomGamma<Real>(b);

		return u / (u + v);
	}

	template <typename Real>
	Real betaPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& a,
		const PASTEL_NO_DEDUCTION(Real)& b)
	{
		return std::pow(x, a - 1) * 
			std::pow(1 - x, b - 1) / beta<Real>(a, b);
	}

}

#endif
