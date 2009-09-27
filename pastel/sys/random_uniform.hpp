#ifndef PASTEL_RANDOM_UNIFORM_HPP
#define PASTEL_RANDOM_UNIFORM_HPP

#include "pastel/sys/random_uniform.h"
#include "pastel/sys/syslibrary.h"

namespace Pastel
{


	inline integer randomInteger()
	{
		// Modified from the official Mersenne Twister source code.
		return (integer)(Pastel::randomUint32() >> 1);
	}

	inline integer randomInteger(integer min, integer max)
	{
		// Note this works correctly even if
		// min = 0 and max = 0x7fffffff, because
		// of the 2-complement wrap-around.

		return (integer)(Pastel::randomUint32() % (uint32)(max - min + 1)) + min;
	}

	template <typename Real>
	Real random()
	{
		// These real versions are due to Isaku Wada, 2002/01/09.
		// Modified from the official Mersenne Twister source code.

		return randomUint32() * ((Real)1.0 / (Real)4294967295.0); 
		// divided by 2^32 - 1
	}

	template <typename Real>
	Real random0()
	{
		// These real versions are due to Isaku Wada, 2002/01/09.
		// Modified from the official Mersenne Twister source code.

		return randomUint32() * ((Real)1.0 / (Real)4294967296.0); 
		// divided by 2^32
	}

	template <typename Real>
	Real random1()
	{
		// These real versions are due to Isaku Wada, 2002/01/09.
		// Modified from the official Mersenne Twister source code.

		return ((Real)randomUint32() + (Real)0.5) * 
			((Real)1.0 / (Real)4294967296.0); 
		// divided by 2^32
	}

	template <typename Real>
	Real random53() 
	{ 
		// These real versions are due to Isaku Wada, 2002/01/09.
		// Modified from the official Mersenne Twister source code.

		const uint32 a = randomUint32() >> 5;
		const uint32 b = randomUint32() >> 6; 

		return (a * (Real)67108864.0 + b) * 
			((Real)1.0 / (Real)9007199254740992.0); 
	} 

	template <typename Real>
	Real random(
		const PASTEL_NO_DEDUCTION(Real)& minValue, 
		const PASTEL_NO_DEDUCTION(Real)& maxValue)
	{
		return minValue + Pastel::random<Real>() * (maxValue - minValue);
	}

	template <typename Real, int N>
	Vector<Real, N> randomVector()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::randomVector<Real, N>(N);
	}

	template <typename Real, int N>
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

	template <typename Real>
	Real uniformPdf(
		const PASTEL_NO_DEDUCTION(Real)& x)
	{
		if (x < 0 || x > 1)
		{
			return 0;
		}

		return 1;
	}

	template <typename Real>
	Real uniformPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& minValue,
		const PASTEL_NO_DEDUCTION(Real)& maxValue)
	{
		PENSURE_OP(minValue, <, maxValue);

		if (x < minValue || x > maxValue)
		{
			return 0;
		}

		return inverse(maxValue - minValue);
	}

}

#endif
