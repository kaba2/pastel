#ifndef PASTEL_MATH_FUNCTIONS_MORE_HPP
#define PASTEL_MATH_FUNCTIONS_MORE_HPP

#include "pastel/sys/math_functions_more.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Type>
	inline Type square(const Type& that)
	{
		return that * that;
	}

	template <typename Type>
	inline Type clamp(const Type& x,
		const PASTEL_NO_DEDUCTION(Type)& xMin,
		const PASTEL_NO_DEDUCTION(Type)& xMax)
	{
		PENSURE_OP(xMin, <=, xMax);

		if (x < xMin)
		{
			return xMin;
		}

		if (x > xMax)
		{
			return xMax;
		}

		return x;
	}

	template <typename Integer>
	inline bool odd(const Integer& x)
	{
		return (x & 1) == 1;
	}

	template <typename Integer>
	inline bool even(const Integer& x)
	{
		return (x & 1) == 0;
	}

	inline integer mod(integer x, integer n)
	{
		PENSURE_OP(n, >, 0);

		const bool isNegative = x < 0;

		if (isNegative)
		{
			x = -x;
		}

		const integer periodicIndex = x % n;
		if (isNegative && periodicIndex != 0)
		{
			return n - periodicIndex;
		}

		return periodicIndex;
	}

	inline real mod(real x)
	{
		return x - std::floor(x);
	}

	inline real mod(real x, real n)
	{
		PENSURE_OP(n, >, 0);
		return Pastel::mod(x / n) * n;
	}

	inline integer integerLog2(integer that)
	{
		ENSURE_OP(that, >, 0);

		integer current = that;
		integer exponent = 0;

		while (current != 1)
		{
			current >>= 1;
			++exponent;
		}

		return exponent;
	}

	inline bool isPowerOfTwo(integer that)
	{
		PENSURE_OP(that, >=, 0);

		return (that & (that - 1)) == 0;
	}

	template <typename Real>
	inline void realToReal(
		const Real& x,
		const PASTEL_NO_DEDUCTION(Real)& fromMin, 
		const PASTEL_NO_DEDUCTION(Real)& fromMax,
		const PASTEL_NO_DEDUCTION(Real)& toMin, 
		const PASTEL_NO_DEDUCTION(Real)& toMax)
	{
		const Real fromDelta = fromMax - fromMin;
		const Real toDelta = toMax - toMin;
		return toMin + ((x - fromMin) / fromDelta) * toDelta;
	}

}

#endif
