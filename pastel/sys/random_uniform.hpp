#ifndef PASTEL_RANDOM_UNIFORM_HPP
#define PASTEL_RANDOM_UNIFORM_HPP

#include "pastel/sys/random_uniform.h"
#include "pastel/sys/syslibrary.h"

namespace Pastel
{

	namespace Detail_RandomUniform
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
		return Detail_RandomUniform::randomReal32();
	}

	template <typename Real>
	typename boost::enable_if<boost::is_same<Real, real64>, real64>::type 
		random()
	{
		return Detail_RandomUniform::randomReal64();
	}

	template <typename Real>
	Real random(
		const PASTEL_NO_DEDUCTION(Real)& minValue, 
		const PASTEL_NO_DEDUCTION(Real)& maxValue)
	{
		return minValue + Pastel::random<Real>() * (maxValue - minValue);
	}

	template <int N, typename Real>
	Vector<Real, N> randomVector()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomVector<N, Real>(N);
	}

	template <int N, typename Real>
	Vector<Real, N> randomVector(integer dimension)
	{
		PENSURE_OP(dimension, >=, 0);

		Vector<Real, N> direction(ofDimension(dimension));

		for (integer i = 0;i < dimension;++i)
		{
			direction[i] = random<Real>();
		}

		return direction;
	}

}

#endif
