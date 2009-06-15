#ifndef PASTEL_RANDOM_VECTOR_HPP
#define PASTEL_RANDOM_VECTOR_HPP

#include "pastel/sys/random_vector.h"
#include "pastel/sys/random.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/vector_tools.h"

#include <cmath>

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorCube()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomVectorCube<N, Real>(N);
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorCube(integer dimension)
	{
		PENSURE1(dimension >= 0, dimension);

		Vector<N, Real> direction(ofDimension(dimension));

		for (integer i = 0;i < dimension;++i)
		{
			direction[i] = 2 * random<Real>() - 1;
		}

		return direction.asTemporary();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorSphere()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomVectorSphere<N, Real>(N);
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorSphere(integer dimension)
	{
		// A randomly distributed vector on the
		// unit sphere can be generated as follows.
		// A (0, v)-normal distribution is given by
		// f(x) = (1 / sqrt(v^2 2pi)) e^(-x^2/(2 v^2))
		// Pick n random variables from such a
		// distribution. Now
		// g(x_1, ..., x_n)
		// = f(x_1) ... f(x_n)
		// = (1 / sqrt(v^2 2pi))^n e^(-(x_1^2 + ... + x_n^2)/(2 v^2))
		// Which is again a distribution in R^n.
		// But the g distribution is only
		// dependent on the norm of X = (x_1, ..., x_n), not
		// on its direction, and thus the normalization of X
		// yields a uniformly distributed vector on the unit sphere.

		PENSURE1(dimension >= 0, dimension);

		Vector<N, Real> result(ofDimension(dimension));

		if (dimension > 0)
		{
			Real dotResult = 0;
			do
			{
				for (integer i = 0;i < dimension;++i)
				{
					result[i] = randomGaussian<Real>();
				}
				dotResult = dot(result);
			}
			while(dotResult < 0.001);

			result /= std::sqrt(dotResult);
		}

		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorBall()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomVectorBall<N, Real>(N);
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorBall(
		integer dimension)
	{
		PENSURE1(dimension >= 0, dimension);

		Vector<N, Real> sphere = 
			randomVectorSphere<N, Real>(dimension);

		sphere *= std::pow(random<Real>(), inverse((Real)dimension));

		return sphere.asTemporary();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorAnnulus(
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomVectorAnnulus<N, Real>(
			minRadius, maxRadius, N);
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorAnnulus(
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius,
		integer dimension)
	{
		PENSURE1(dimension >= 0, dimension);

		Vector<N, Real> sphere = 
			randomVectorSphere<N, Real>(dimension);

		sphere *= 
			std::pow(
			linear(
			std::pow(minRadius, (Real)dimension), 
			std::pow(maxRadius, (Real)dimension), 
			random<Real>()),
			inverse((Real)dimension));

		return sphere.asTemporary();
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

	template <int N, typename Real>
	TemporaryVector<N, Real> randomExponentialVector()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::randomExponentialVector<N, Real>(N);
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomExponentialVector(
		integer dimension)
	{
		PENSURE1(dimension >= 0, dimension);

		Vector<N, Real> result(ofDimension(dimension));
		for (integer i = 0;i < dimension;++i)
		{
			result[i] = randomExponential<Real>();
		}
		
		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomGammaVector(
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{		
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::randomGammaVector<N, Real>(
			N, shape, scale);
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> randomGammaVector(
		integer dimension,
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{
		PENSURE1(dimension >= 0, dimension);

		Vector<N, Real> result(ofDimension(dimension));
		for (integer i = 0;i < dimension;++i)
		{
			result[i] = randomGamma<Real>(shape);
		}
		
		return result.asTemporary();
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
