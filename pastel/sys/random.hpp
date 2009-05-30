#ifndef PASTEL_RANDOM_HPP
#define PASTEL_RANDOM_HPP

#include "pastel/sys/random.h"

namespace Pastel
{

	namespace Detail_Random
	{

		PASTELSYS real32 randomReal32();
		PASTELSYS real64 randomReal64();

		PASTELSYS real32 randomGaussianReal32();
		PASTELSYS real64 randomGaussianReal64();

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

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVector()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomVector<N, Real>(N);
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVector(integer dimension)
	{
		PENSURE1(dimension >= 0, dimension);

		Vector<N, Real> direction(ofDimension(dimension));

		for (integer i = 0;i < dimension;++i)
		{
			direction[i] = random<Real>();
		}

		return direction.asTemporary();
	}

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real32>, real32>::type 
		randomGaussian()
	{
		return Detail_Random::randomGaussianReal32();
	}

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real64>, real64>::type 
		randomGaussian()
	{
		return Detail_Random::randomGaussianReal64();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomGaussianVector()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomGaussianVector<N, Real>(N);
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomGaussianVector(integer dimension)
	{
		PENSURE1(dimension >= 0, dimension);
		Vector<N, Real> direction(ofDimension(dimension));

		for (integer i = 0;i < dimension;++i)
		{
			direction[i] = randomGaussian<Real>();			
		}

		return direction.asTemporary();
	}

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real32>, real32>::type 
		randomExponential()
	{
		return Detail_Random::randomExponentialReal32();
	}

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real64>, real64>::type 
		randomExponential()
	{
		return Detail_Random::randomExponentialReal64();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomExponentialVector(
		const PASTEL_NO_DEDUCTION(Real)& shape, 
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::randomExponentialVector<N, Real>(
			N, shape, scale);
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomExponentialVector(
		integer dimension,
		const PASTEL_NO_DEDUCTION(Real)& shape, 
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		PENSURE1(dimension >= 0, dimension);

		Vector<N, Real> result(ofDimension(dimension));
		for (integer i = 0;i < dimension;++i)
		{
			result[i] = randomExponential<Real>(shape, scale);
		}
		
		return result.asTemporary();
	}

	template <typename Real>
	Real randomGamma(PASTEL_NO_DEDUCTION(Real) alpha)
	{
		// See "Sampling from the Gamma Distribution on a Computer",
		// George F. Fishman, 1976.

		const integer maxIterations = 100;

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

	template <int N, typename Real>
	TemporaryVector<N, Real> randomGammaVector(
		const PASTEL_NO_DEDUCTION(Real)& shape, 
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::randomGammaVector<N, Real>(
			N, shape, scale);
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomGammaVector(
		integer dimension,
		const PASTEL_NO_DEDUCTION(Real)& shape, 
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		PENSURE1(dimension >= 0, dimension);

		Vector<N, Real> result(ofDimension(dimension));
		for (integer i = 0;i < dimension;++i)
		{
			result[i] = randomGamma<Real>(shape, scale);
		}
		
		return result.asTemporary();
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

	template <int N, typename Real>
	TemporaryVector<N, Real> randomGeneralizedGaussianVector(
		const PASTEL_NO_DEDUCTION(Real)& shape, 
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::randomGeneralizedGaussianVector<N, Real>(
			N, shape, scale);
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomGeneralizedGaussianVector(
		integer dimension,
		const PASTEL_NO_DEDUCTION(Real)& shape, 
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		PENSURE1(dimension >= 0, dimension);

		Vector<N, Real> result(ofDimension(dimension));
		for (integer i = 0;i < dimension;++i)
		{
			result[i] = randomGeneralizedGaussian<Real>(shape, scale);
		}
		
		return result.asTemporary();
	}


}

#endif
