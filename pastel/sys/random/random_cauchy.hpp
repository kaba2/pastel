#ifndef PASTELSYS_RANDOM_CAUCHY_HPP
#define PASTELSYS_RANDOM_CAUCHY_HPP

#include "pastel/sys/random/random_cauchy.h"
#include "pastel/sys/random/random_gaussian.h"
#include "pastel/sys/math/constants.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real>
	Real randomCauchy()
	{
		Real u = randomGaussian<Real>();
		
		Real v = 0;
		do
		{
			v = randomGaussian<Real>();
		}
		while (v == 0);

		return u / v;
	}

	template <typename Real>
	Real randomCauchy(
		const NoDeduction<Real>& scale)
	{
		PENSURE_OP(scale, >, 0);
		return randomCauchy<Real>() * scale;
	}

	template <typename Real>
	Real cauchyPdf(
		const NoDeduction<Real>& x)
	{
		return inverse(constantPi<Real>() * (1 + square(x)));
	}

	template <typename Real>
	Real cauchyPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& scale)
	{
		PENSURE_OP(scale, >, 0);

		Real invScale = inverse(scale);

		return Pastel::cauchyPdf<Real>(x * invScale) * invScale;
	}

}

#endif
