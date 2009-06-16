#ifndef PASTEL_MATH_FUNCTIONS_MORE_HPP
#define PASTEL_MATH_FUNCTIONS_MORE_HPP

#include "pastel/sys/math_functions_more.h"
#include "pastel/sys/ensure.h"

#include <boost/static_assert.hpp>

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
		PENSURE(xMin <= xMax);

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
		PENSURE1(n > 0, n);

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

	inline integer integerLog2(integer that)
	{
		ENSURE1(that > 0, that);

		integer current = that;
		integer exponent = 0;

		while (current != 1)
		{
			current >>= 1;
			++exponent;
		}

		return exponent;
	}

	inline integer roundUpTo(integer that, integer to)
	{
		PENSURE1(to >= 0, to);

		const integer remainder = mod(that, to);
		if (remainder > 0)
		{
			return that + (to - remainder);
		}

		return that;
	}

	inline integer roundUpToOdd(integer that)
	{
		if (odd(that))
		{
			return that;
		}

		return that + 1;
	}

	inline integer roundUpToOdd(real that)
	{
		return Pastel::roundUpToOdd((integer)std::ceil(that));
	}

	inline integer roundUpToEven(integer that)
	{
		if (odd(that))
		{
			return that + 1;
		}

		return that;
	}

	inline integer roundUpToEven(real that)
	{
		return Pastel::roundUpToEven((integer)std::ceil(that));
	}

	inline integer roundUpToPowerOf2(integer that, integer power)
	{
		PENSURE1(that >= 0, that);
		PENSURE1(power >= 0, power);

		const integer to = (1 << power);
		const integer remainder = that & (to - 1);

		if (remainder > 0)
		{
			return that + (to - remainder);
		}

		return that;
	}

	inline integer roundUpToPowerOf2(integer that)
	{
		BOOST_STATIC_ASSERT(sizeof(integer) == sizeof(int32));

		PENSURE1(that >= 0, that);

		// NOTE: Works only for 32 bit integer.

		--that;

		that |= that >> 1;
		that |= that >> 2;
		that |= that >> 4;
		that |= that >> 8;
		that |= that >> 16;

		return that + 1;
	}

	inline bool isPowerOfTwo(integer that)
	{
		PENSURE1(that >= 0, that);

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

	inline integer quantizeUnsigned(real64 x, integer numbers)
	{
		return (integer)clamp((integer)(x * numbers), 0, numbers - 1);
	}

	inline real64 dequantizeUnsignedMatchEnds(integer x, integer numbers)
	{
		return clamp((real64)x / (numbers - 1), 0, 1);
	}

	inline real64 dequantizeUnsigned(integer x, integer numbers)
	{
		return clamp((((real64)x) + 0.5) / numbers, 0, 1);
	}

	template <int N>
	inline real64 dequantizeSigned(integer i)
	{
		BOOST_STATIC_ASSERT(N >= 2 && N <= 32);

		enum
		{
			Half = (1 << (N - 1)) - 1
		};
		
		return clamp((real64)i / Half, -1, 1);
	}

	/*
	template <int N>
	inline real64 ditheredQuantizeSigned(integer i)
	{
		BOOST_STATIC_ASSERT(N >= 2 && N <= 32);

		enum
		{
			Half = (1 << (N - 1)) - 1
		};
		
		real64 dither = random<real64>();
		if (i <= -Half)
		{
			dither = 0;
		}
		if (i >= Half)
		{
			dither = 1;
		}
		
		return clamp((real64)(i + dither) / (Half + 1), -1, 1);
	}
	*/

	template <int N>
	inline integer quantizeSigned(real64 r)
	{
		BOOST_STATIC_ASSERT(N >= 2 && N <= 32);

		enum
		{
			Half = (1 << (N - 1)) - 1
		};

		return (integer)clamp(std::floor(r * Half + (r + 1) / 2), -Half, Half);
	}

}

#endif
