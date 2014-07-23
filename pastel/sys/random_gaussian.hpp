#ifndef PASTELSYS_RANDOM_GAUSSIAN_HPP
#define PASTELSYS_RANDOM_GAUSSIAN_HPP

#include "pastel/sys/random_gaussian.h"
#include "pastel/sys/random_uniform.h"
#include "pastel/sys/constants.h"

#include <boost/math/distributions/normal.hpp>

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
		const PASTEL_NO_DEDUCTION(Real)& deviation)
	{
		PENSURE_OP(deviation, >=, 0);

		return Pastel::randomGaussian<Real>() * deviation;
	}

	template <typename Real, int N>
	Vector<Real, N> randomGaussianVector()
	{
		PASTEL_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomGaussianVector<Real, N>(N);
	}

	template <typename Real, int N>
	Vector<Real, N> randomGaussianVector(integer dimension)
	{
		PENSURE_OP(dimension, >=, 0);
		Vector<Real, N> direction(ofDimension(dimension));

		for (integer i = 0;i < dimension;++i)
		{
			direction[i] = randomGaussian<Real>();			
		}

		return direction;
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
		const PASTEL_NO_DEDUCTION(Real)& deviation)
	{
		PENSURE_OP(deviation, >=, 0);

		return inverse(deviation * std::sqrt(2 * constantPi<Real>())) * 
			std::exp(-square(x / deviation) / 2);
	}

	template <typename Real>
	Real approximateGaussianCdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& deviation)
	{
		return Pastel::approximateGaussianCdf<Real>(
			x / deviation);
	}

	template <typename Real>
	Real approximateGaussianCdf(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		// "Handbook and Mathematical Functions",
		// Abramowitz and Stegun, 1964.
		//
		// Formula 26.2.17.
		//
		// |Error| <= 7.5 * 10^-8

		Real b0 = 0.2316419;
		Real b1 = 0.319381530;
		Real b2 = -0.356563782;
		Real b3 = 1.781477937;
		Real b4 = -1.821255978;
		const Real b5 = 1.330274429;
		const Real xAbs = std::abs(x);

		const Real t = inverse(1 + b0 * xAbs);
		
		//Real result = gaussianPdf<Real>(xAbs) * (
		//	t * (b1 + t * (b2 + t * (b3 + t * (b4 + t * b5)))));

		Real t2 = square(t);

		const Real t3 = t2 * t;
		Real t4 = square(t2);

		const Real t5 = t4 * t;

		Real result = gaussianPdf<Real>(xAbs) * (
			t * b1 + t2 * b2 + t3 * b3 + t4 * b4 + t5 * b5);

		if (x > 0)
		{
			result = (Real)1 - result;
		}

		return result;
	}

	template <typename Real>
	Real gaussianCdf(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		boost::math::normal_distribution<Real>
			normal;

		return boost::math::cdf(normal, x);
	}

	template <typename Real>
	Real gaussianCdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& deviation)
	{
		return Pastel::gaussianCdf<Real>(
			x / deviation);
	}

}

#endif
