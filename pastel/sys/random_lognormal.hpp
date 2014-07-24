#ifndef PASTELSYS_RANDOM_LOGNORMAL_HPP
#define PASTELSYS_RANDOM_LOGNORMAL_HPP

#include "pastel/sys/random_lognormal.h"
#include "pastel/sys/random_gaussian.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real>
	Real randomLogNormal()
	{
		Real u = randomGaussian<Real>();
		return std::exp(u);
	}

	template <typename Real>
	Real randomLogNormal(
		const PASTEL_NO_DEDUCTION(Real)& logMean,
		const PASTEL_NO_DEDUCTION(Real)& logDeviation)
	{
		PENSURE_OP(logDeviation, >, 0);

		Real u = randomGaussian<Real>(logDeviation) + logMean;
		return std::exp(u);
	}

	template <typename Real>
	Real logNormalPdf(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		return inverse(x * std::sqrt(2 * constantPi<Real>())) * 
			std::exp(-square(std::log(x)) / 2);
	}

	template <typename Real>
	Real logNormalPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& logMean,
		const PASTEL_NO_DEDUCTION(Real)& logDeviation)
	{
		PENSURE_OP(logDeviation, >, 0);

		if (x == 0)
		{
			return 0;
		}

		Real u =
			std::exp(-square(std::log(x) - logMean) / (2 * square(logDeviation)));

		Real v =
			x * logDeviation * std::sqrt(2 * constantPi<Real>());

		return u / v;
	}

}

#endif
