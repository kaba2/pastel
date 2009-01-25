#ifndef PASTELSYS_NATIVEREAL_HPP
#define PASTELSYS_NATIVEREAL_HPP

#include "pastel/sys/nativereal.h"
#include "pastel/sys/ensure.h"

#include <cmath>
#include <limits>

namespace Pastel
{

	inline double infinity(double*)
	{
		return std::numeric_limits<double>::infinity();
	}

	inline double nan(double*)
	{
		return std::numeric_limits<double>::quiet_NaN();
	}

	inline double epsilon(double*)
	{
		static double theEpsilon = 1e-6;

		return theEpsilon;
	}

	inline double epsilon2(double*)
	{
		static double theEpsilon2 = 1e-12;

		return theEpsilon2;
	}

	inline double epsilon3(double*)
	{
		static double theEpsilon3 = 1e-18;

		return theEpsilon3;
	}

	template <typename Integer>
	Integer integerCast(double x)
	{
		return Integer(x);
	}

	// Optimization functions

	inline void multiplyByPowerOf2(double& x, double power)
	{
		x *= std::pow((double)2, power);
	}

	inline void divideByPowerOf2(double& x, double power)
	{
		x /= std::pow((double)2, power);
	}

	inline double inverse(double x)
	{
		return 1 / x;
	}

	inline bool zero(double x)
	{
		return x == 0;
	}

	inline bool negative(double x)
	{
		return x < 0;
	}

	inline bool positive(double x)
	{
		return x > 0;
	}

	// Float

	inline float infinity(float*)
	{
		return std::numeric_limits<float>::infinity();
	}

	inline float nan(float*)
	{
		return std::numeric_limits<float>::quiet_NaN();
	}

	inline float epsilon(float*)
	{
		static float theEpsilon = 1e-3;

		return theEpsilon;
	}

	inline float epsilon2(float*)
	{
		static float theEpsilon2 = 1e-6;

		return theEpsilon2;
	}

	inline float epsilon3(float*)
	{
		static float theEpsilon3 = 1e-9;

		return theEpsilon3;
	}

	template <typename Integer>
	Integer integerCast(float x)
	{
		return Integer(x);
	}

	// Optimization functions

	inline void multiplyByPowerOf2(float& x, float power)
	{
		x *= std::pow((float)2, power);
	}

	inline void divideByPowerOf2(float& x, float power)
	{
		x /= std::pow((float)2, power);
	}

	inline float inverse(float x)
	{
		return 1 / x;
	}

	inline bool zero(float x)
	{
		return x == 0;
	}

	inline bool negative(float x)
	{
		return x < 0;
	}

	inline bool positive(float x)
	{
		return x > 0;
	}

}

#endif
