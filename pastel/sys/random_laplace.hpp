#ifndef PASTELSYS_RANDOM_LAPLACE_HPP
#define PASTELSYS_RANDOM_LAPLACE_HPP

#include "pastel/sys/random_laplace.h"
#include "pastel/sys/random_uniform.h"
#include "pastel/sys/random_exponential.h"

namespace Pastel
{

	template <typename Real>
	Real randomLaplace()
	{
		/*
		Laplace distribution is simply a 
		two-sided Exponential distribution.
		See the derivation for Exponential
		distribution. We could call its
		generation function, but doing it
		this way saves us generating one 
		additional random variable.
		*/

		Real u = 0;
		do
		{
			u = random<Real>(-1, 1);
		}
		while(u == 0);

		if (u < 0)
		{
			// Negative side.
			return std::log(-u);
		}

		// Positive side.
		return -std::log(u);
	}

	template <typename Real>
	Real randomLaplace(
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		return Pastel::randomLaplace<Real>() * scale;
	}

	template <typename Real>
	Real laplacePdf(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		return exponentialPdf<Real>(std::abs(x)) / 2;
	}

	template <typename Real>
	Real laplacePdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		const Real invScale = inverse(scale);

		return Pastel::laplacePdf<Real>(x * invScale) * invScale;
	}

}

#endif
