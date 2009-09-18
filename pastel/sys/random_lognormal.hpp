#ifndef PASTEL_RANDOM_LOGNORMAL_HPP
#define PASTEL_RANDOM_LOGNORMAL_HPP

#include "pastel/sys/random_lognormal.h"
#include "pastel/sys/random_gaussian.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real>
	Real randomLogNormal()
	{
		const Real u = randomGaussian<Real>();
		return std::exp(u);
	}

	template <typename Real>
	Real randomLogNormal(
		const PASTEL_NO_DEDUCTION(Real)& normalMean,
		const PASTEL_NO_DEDUCTION(Real)& normalDeviation)
	{
		PENSURE_OP(normalDeviation, >, 0);

		const Real u = randomGaussian<Real>(normalDeviation) + normalMean;
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
		const PASTEL_NO_DEDUCTION(Real)& normalMean,
		const PASTEL_NO_DEDUCTION(Real)& normalDeviation)
	{
		return inverse(x * normalDeviation * std::sqrt(2 * constantPi<Real>())) * 
			std::exp(-square(std::log(x) - normalMean) / (2 * square(normalDeviation)));
	}

}

#endif
