#ifndef PASTEL_RANDOM_GAMMA_HPP
#define PASTEL_RANDOM_GAMMA_HPP

#include "pastel/sys/random_gamma.h"

namespace Pastel
{

	template <typename Real>
	Real randomGamma(
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{
		// See "A simple method for generating gamma variables",
		// George Marsaglia, Wai Wan Tsang,
		// ACM Transactions on Mathematical Software (TOMS)
		// Volume 26, Issue 3 (September 2000).

		// We assume scale = 1 without loss of generality.
		// This is a simple scaling of the random deviate,
		// which we leave for the user. This improves
		// performance.

		PENSURE_OP(shape, >, 0);
		
		const Real modifiedShape = (shape < 1) ? shape + 1 : shape;

		const Real d = modifiedShape - (Real)1 / 3;
		const Real c = 1 / std::sqrt(9 * d);

		Real v = 0;
		while(true)
		{
			Real x = 0;
			do
			{
				x = randomGaussian<Real>();
				v = 1 + c * x;
			}
			while(v <= 0);

			v *= v * v;
			x *= x;

			Real u = random<Real>();

			if (u < 1 - 0.331 * square(x) ||
				std::log(u) < 0.5 * x + d * (1 - v + std::log(v)))
			{
				break;
			}
		}
		
		if (shape < 1)
		{
			// shape < 1 is problematic
			// because the density function of the
			// gamma distribution is not bounded.
			// This is handled by noticing that if
			//
			// y ~ gamma(shape + 1, 1)
			// u ~ uniform(0, 1)
			//
			// then
			//
			// y u^(1 / shape) ~ gamma(shape, 1)

			Real u = 0;
			do
			{
				u = random<Real>();
			}
			while(u == 0);

			return std::pow(u, 1 / shape) * d * v;
		}
		
		return d * v;
	}

	template <typename Real>
	Real randomGamma(
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		PENSURE_OP(shape, >, 0);
		PENSURE_OP(scale, >= , 0);

		return Pastel::randomGamma<Real>(shape) * scale;
	}

	template <typename Real>
	Real varianceToGammaScale(
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& variance)
	{
		PENSURE_OP(shape, >, 0);
		PENSURE_OP(variance, >= , 0);

		return std::sqrt(variance / shape);
	}

	template <int N, typename Real>
	Vector<Real, N> randomGammaVector(
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{		
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::randomGammaVector<N, Real>(
			N, shape, scale);
	}

	template <int N, typename Real>
	Vector<Real, N> randomGammaVector(
		integer dimension,
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{
		PENSURE_OP(dimension, >=, 0);

		Vector<Real, N> result(ofDimension(dimension));
		for (integer i = 0;i < dimension;++i)
		{
			result[i] = randomGamma<Real>(shape);
		}
		
		return result;
	}

	template <typename Real>
	Real gammaPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{
		PENSURE_OP(shape, >, 0);

		return std::pow(x, shape - 1) * 
			std::exp(-x) / gamma<Real>(shape);
	}

	template <typename Real>
	Real gammaPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		PENSURE_OP(shape, >, 0);
		PENSURE_OP(scale, >, 0);

		return std::pow(x, shape - 1) * 
			std::exp(-x / scale) / 
			(gamma<Real>(shape) * std::pow(scale, shape));
	}

	// This version does not seem to always work
	// (very large number of iterations never pass the 
	// rejection loop). 
	/*
	template <typename Real>
	Real randomGamma(const PASTEL_NO_DEDUCTION(Real)& alpha)
	{
		// See "Sampling from the Gamma Distribution on a Computer",
		// George F. Fishman, 1976.

		const integer maxIterations = 1000;

		for (integer i = 0;i < maxIterations;++i)
		{
			const Real randExp = randomExponential<Real>();
			const Real randUni = random<Real>();

			if (randUni < std::pow(randExp / std::exp(randExp + 1), alpha - 1))
			{
				return alpha * randExp;
			}
		}

		const bool gammaDistributionSamplingFailed = true;
		REPORT(gammaDistributionSamplingFailed);
		
		return 0;
	}
	*/

}

#endif
