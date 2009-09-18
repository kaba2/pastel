#ifndef PASTEL_RANDOM_LAPLACE_HPP
#define PASTEL_RANDOM_LAPLACE_HPP

#include "pastel/sys/random_laplace.h"
#include "pastel/sys/random_uniform.h"

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

		const Real u = random<Real>(-1, 1);
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

}

#endif
