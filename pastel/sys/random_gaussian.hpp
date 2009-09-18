#ifndef PASTEL_RANDOM_GAUSSIAN_HPP
#define PASTEL_RANDOM_GAUSSIAN_HPP

#include "pastel/sys/random_gaussian.h"
#include "pastel/sys/random_uniform.h"

namespace Pastel
{

	template <typename Real>
	Real randomGaussian()
	{
		/*
		The probability density distribution of the
		gaussian distribution is

		p(x) = (1 / sqrt(d^2 2 pi)) e^(-(x - m)^2 / (2d^2))
		
		where d is the standard deviation
		and m is the mean.

		Without loss of generality, we take
		d = 1 and m = 0, and let the user transform the
		variable to the desired parameters.
		Then the pdf is given by:

		p(x) = (1 / sqrt(2 pi)) e^(-x^2 / 2)

		See the 'Box-Muller transform' for the
		generation algorithm.
		*/

		Real u = 0;
		Real v = 0;
		Real s = 0;
		
		do
		{
			u = 2 * random<Real>() - 1;
			v = random<Real>();
			s = u * u + v * v;
		}
		while(s <= 0 || s >= 1);

		return u * std::sqrt(-2 * std::log(s) / s);
	}

	template <typename Real>
	Real randomGaussian(
		const PASTEL_NO_DEDUCTION(Real)& mean,
		const PASTEL_NO_DEDUCTION(Real)& deviation)
	{
		PENSURE_OP(deviation, >=, 0);

		return Pastel::randomGaussian<Real>() * deviation + mean;
	}

	template <typename Real>
	Real gaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		return inverse(std::sqrt(2 * constantPi<Real>())) * 
			std::exp(-square(x) / 2);
	}

	template <typename Real>
	Real gaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& mean,
		const PASTEL_NO_DEDUCTION(Real)& deviation)
	{
		PENSURE_OP(deviation, >=, 0);

		return inverse(deviation * std::sqrt(2 * constantPi<Real>())) * 
			std::exp(-square(x / deviation) / 2);
	}

}

#endif
