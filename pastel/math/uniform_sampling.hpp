#ifndef PASTEL_UNIFORM_SAMPLING_HPP
#define PASTEL_UNIFORM_SAMPLING_HPP

#include "pastel/math/uniform_sampling.h"
#include "pastel/sys/random.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/vector_tools.h"

#include <cmath>

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real>
	Vector<Real, N> randomVectorCube()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomVectorCube<N, Real>(N);
	}

	template <int N, typename Real>
	Vector<Real, N> randomVectorCube(integer dimension)
	{
		PENSURE_OP(dimension, >=, 0);

		Vector<Real, N> direction(ofDimension(dimension));

		for (integer i = 0;i < dimension;++i)
		{
			direction[i] = 2 * random<Real>() - 1;
		}

		return direction;
	}

	template <int N, typename Real>
	Vector<Real, N> randomVectorSphere()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomVectorSphere<N, Real>(N);
	}

	template <int N, typename Real>
	Vector<Real, N> randomVectorSphere(integer dimension)
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

		PENSURE_OP(dimension, >=, 0);

		Vector<Real, N> result(ofDimension(dimension));

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

		return result;
	}

	template <int N, typename Real>
	Vector<Real, N> randomVectorBall()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomVectorBall<N, Real>(N);
	}

	template <int N, typename Real>
	Vector<Real, N> randomVectorBall(
		integer dimension)
	{
		PENSURE_OP(dimension, >=, 0);

		Vector<Real, N> sphere = 
			randomVectorSphere<N, Real>(dimension);

		sphere *= std::pow(random<Real>(), inverse((Real)dimension));

		return sphere;
	}

	template <int N, typename Real>
	Vector<Real, N> randomVectorAnnulus(
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomVectorAnnulus<N, Real>(
			minRadius, maxRadius, N);
	}

	template <int N, typename Real>
	Vector<Real, N> randomVectorAnnulus(
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius,
		integer dimension)
	{
		PENSURE_OP(dimension, >=, 0);

		Vector<Real, N> sphere = 
			randomVectorSphere<N, Real>(dimension);

		sphere *= 
			std::pow(
			linear(
			std::pow(minRadius, (Real)dimension), 
			std::pow(maxRadius, (Real)dimension), 
			random<Real>()),
			inverse((Real)dimension));

		return sphere;
	}

}

#endif
