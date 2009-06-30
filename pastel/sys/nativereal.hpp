#ifndef PASTEL_NATIVEREAL_HPP
#define PASTEL_NATIVEREAL_HPP

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
