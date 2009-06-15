#ifndef PASTEL_RANDOM_HPP
#define PASTEL_RANDOM_HPP

#include "pastel/sys/random.h"

namespace Pastel
{

	namespace Detail_Random
	{

		PASTELSYS real32 randomReal32();
		PASTELSYS real64 randomReal64();

		PASTELSYS real32 randomExponentialReal32();
		PASTELSYS real64 randomExponentialReal64();

	}

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real32>, real32>::type 
		random()
	{
		return Detail_Random::randomReal32();
	}

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real64>, real64>::type 
		random()
	{
		return Detail_Random::randomReal64();
	}

	template <typename Real>
	Real random(
		const PASTEL_NO_DEDUCTION(Real)& minValue, 
		const PASTEL_NO_DEDUCTION(Real)& maxValue)
	{
		return minValue + Pastel::random<Real>() * (maxValue - minValue);
	}

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

		See the Box-Muller transform for the
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
	Real randomExponential()
	{
		/*
		The probability density distribution of the
		exponential distribution is

		p(x) = a e^(-ax), for x >= 0
		     = 0        , for x <  0

		where a > 0.

		Its cumulative distribution is easy to derive:

		for x < 0:
		c(x) = 0

		for x >= 0:
		c(x) = int[t = 0..x] p(t) dt
		     = int[t = 0..x] a e^(-at) dt
			 = -[t = 0..x] e^(-at) dt
			 = -(e^(-ax) - 1)
			 = 1 - e^(-ax)
	    
		(For a check, c(oo) = 1)
		
		Thus, the inverse of the cdf (for x >= 0) is
		derived by:

		t = 1 - e^(-ax)
		<=>
		1 - t = e^(-ax)
		<=>
		ln(1 - t) = -ax
		<=>
		x = -ln(1 - t) / a

		So if we pick t a uniform random number in [0, 1],
		then -ln(1 - t) / a is exponentially distributed.
		We simplify this further as follows. First, if t is 
		uniformly distributed in [0, 1], then so is
		1 - t. Second, we can assume a = 1 without loss of
		generality and let the user do the scaling
		if needed. This improves performance.
		*/

		return -std::log(random<Real>());
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

	template <typename Real>
	Real randomGamma(const PASTEL_NO_DEDUCTION(Real)& alpha)
	{
		// See "Numerical Recipes, The art of scientific computing", 
		// 3rd. ed, page 370.

		// We assume beta = 1 without loss of generality.
		// This is a simple scaling of the random deviate,
		// which we leave for the user. This improves
		// performance.

		PENSURE(alpha > 0);
		
		const Real modifiedAlpha = (alpha < 1) ? alpha + 1 : alpha;

		const Real a1 = modifiedAlpha - (Real)1 / 3;
		const Real a2 = 1 / std::sqrt(9 * a1);

		Real u = 0;
		Real v = 0;
		Real x = 0;
		do
		{
			do
			{
				x = randomGaussian<Real>();
				v = 1 + a2 * x;
			}
			while(v <= 0);

			v *= v * v;
			u = random<Real>();
		}
		while(u > 1 - 0.331 * std::sqrt(std::sqrt(x)) &&
			std::log(u) > 0.5 * std::sqrt(x) + a1 * (1 - v + std::log(v)));
		
		if (alpha < 1)
		{
			// alpha < 1 is problematic
			// because the density function of the
			// gamma distribution is not bounded.
			// This is handled by noticing that if
			//
			// y ~ gamma(alpha + 1, 1)
			// u ~ uniform(0, 1)
			//
			// then
			//
			// y u^(1 / alpha) ~ gamma(alpha, 1)

			do
			{
				u = random<Real>();
			}
			while(u == 0);

			return std::pow(u, 1 / alpha) * a1 * v;
		}
		
		return a1 * v;
	}

	template <typename Real>
	Real randomGeneralizedGaussian(
		const PASTEL_NO_DEDUCTION(Real)& shape, 
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		// See "Computer generation of the exponential power
		// distributions", Johnson, M. E., 
		// Journal of Statistical Computation and Simulation, 9,
		// pp. 239--240, 1979

		const Real invShape = 1 / shape;
		const Real x = std::pow(std::abs(
			randomGamma<Real>(invShape) * scale), invShape);
		
		if (random<Real>() < 0.5)
		{
			return -x;
		}

		return x;
	}

}

#endif
