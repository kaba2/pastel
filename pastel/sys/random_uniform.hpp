#ifndef PASTELSYS_RANDOM_UNIFORM_HPP
#define PASTELSYS_RANDOM_UNIFORM_HPP

#include "pastel/sys/random_uniform.h"
#include "pastel/sys/syslibrary.h"
#include "pastel/sys/random_integer.h"

namespace Pastel
{


	template <typename Real>
	Real random()
	{
		// Divided by 2^32 - 1
		return randomUint32() * ((Real)1.0 / (Real)4294967295.0); 
	}

	template <typename Real>
	Real randomOpen()
	{
		// Divided by 2^32
		return ((Real)randomUint32() + (Real)0.5) * 
			((Real)1.0 / (Real)4294967296.0); 
	}

	template <typename Real>
	Real randomOpen0()
	{
		// Divided by 2^32
		return ((Real)randomUint32() + 1) * 
			((Real)1.0 / (Real)4294967296.0); 
	}

	template <typename Real>
	Real randomOpen1()
	{
		// Divided by 2^32
		return randomUint32() * ((Real)1.0 / (Real)4294967296.0); 
	}

	template <typename Real>
	Real random53() 
	{ 
		// These real versions are due to Isaku Wada, 2002/01/09.
		// Modified from the official Mersenne Twister source code.

		uint32 a = randomUint32() >> 5;
		uint32 b = randomUint32() >> 6; 


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
		PASTEL_STATIC_ASSERT(N != Dynamic);
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
