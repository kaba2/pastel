#ifndef PASTELSYS_RANDOM_GAMMA_HPP
#define PASTELSYS_RANDOM_GAMMA_HPP

#include "pastel/sys/random_gamma.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real>
	Real randomGamma(
		const NoDeduction<Real>& shape)
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
		
		Real modifiedShape = (shape < 1) ? shape + 1 : shape;

		Real d = modifiedShape - (Real)1 / 3;

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

			Real u = randomOpen0<Real>();


			return std::pow(u, 1 / shape) * d * v;
		}
		
		return d * v;
	}

	template <typename Real>
	Real randomGamma(
		const NoDeduction<Real>& shape,
		const NoDeduction<Real>& scale)
	{
		return Pastel::randomGamma<Real>(shape) * scale;
	}

	template <typename Real>
	Real varianceToGammaScale(
		const NoDeduction<Real>& shape,
		const NoDeduction<Real>& variance)
	{
		PENSURE_OP(shape, >, 0);
		PENSURE_OP(variance, >= , 0);

		return std::sqrt(variance / shape);
	}

	template <typename Real, int N>
	Vector<Real, N> randomGammaVector(
		const NoDeduction<Real>& shape)
	{		
		PASTEL_STATIC_ASSERT(N != Dynamic);

		return Pastel::randomGammaVector<Real, N>(
			N, shape);
	}

	template <typename Real, int N>
	Vector<Real, N> randomGammaVector(
		integer dimension,
		const NoDeduction<Real>& shape)
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
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& shape)
	{
		PENSURE_OP(shape, >, 0);

		return std::pow(x, shape - 1) * 
			std::exp(-x) / gamma<Real>(shape);
	}

	template <typename Real>
	Real gammaPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& shape,
		const NoDeduction<Real>& scale)
	{
		PENSURE_OP(shape, >, 0);
		PENSURE_OP(scale, >, 0);

		/*
		return std::pow(x, shape - 1) * 
			std::exp(-x / scale) / 
			(gamma<Real>(shape) * std::pow(scale, shape));
		*/

		Real logPdf =
			((shape - 1) * std::log(x) - (x / scale)) -
			(lnGamma<Real>(shape) + shape * std::log(scale));

		return std::exp(logPdf);
	}

	// This version does not seem to always work
	// (very large number of iterations never pass the 
	// rejection loop). 
	/*
	template <typename Real>
	Real randomGamma(const NoDeduction<Real>& alpha)
	{
		// See "Sampling from the Gamma Distribution on a Computer",
		// George F. Fishman, 1976.

		integer maxIterations = 1000;

		for (integer i = 0;i < maxIterations;++i)
		{
			Real randExp = randomExponential<Real>();
			Real randUni = random<Real>();

			if (randUni < std::pow(randExp / std::exp(randExp + 1), alpha - 1))
			{

				return alpha * randExp;
			}
		}

		bool gammaDistributionSamplingFailed = true;
		REPORT(gammaDistributionSamplingFailed);
		
		return 0;
	}

	*/

}

#endif
