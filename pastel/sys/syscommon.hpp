#ifndef PASTELSYS_SYSCOMMON_HPP
#define PASTELSYS_SYSCOMMON_HPP

#include "pastel/sys/syscommon.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"
#include "pastel/sys/random.h"

#include <sstream>
#include <string>

namespace Pastel
{

	inline integer toPixelSpanPoint(real t)
	{
		return std::ceil(t - (real)0.5);
	}

	template <int N>
	inline Point<N, integer> toPixelSpanPoint(
		const Point<N, real>& that)
	{
		Point<N, integer> result;
		for (integer i = 0;i < N;++i)
		{
			result[i] = Pastel::toPixelSpanPoint(that[i]);
		}

		return result;
	}

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

	inline integer quantizeUnsigned(real64 number, integer max)
	{
		return (integer)clamp((number * (max + 1)), 0, max);
	}

	inline real64 dequantizeUnsigned(integer number, integer max)
	{
		return clamp((real64)number / max, 0, 1);
	}

	inline real64 dequantize(integer i,
					   integer minInteger, integer maxInteger,
					   real64 minReal, real64 maxReal)
	{
		PENSURE3(i >= minInteger && i <= maxInteger, i, minInteger, maxInteger);

		const integer deltaInteger = maxInteger - minInteger;

		if (deltaInteger == 0)
		{
			return (minReal + maxReal) / 2;
		}

		const real64 deltaReal = maxReal - minReal;
		const integer iClamped = clamp(i, minInteger, maxInteger);

		return minReal + ((real64)(iClamped - minInteger) / deltaInteger) * deltaReal;
	}

	inline real64 ditheredQuantize(integer i,
					   integer minInteger, integer maxInteger,
					   real64 minReal, real64 maxReal)
	{
		const integer deltaInteger = maxInteger - minInteger;
		const real64 deltaReal = maxReal - minReal;
		const integer iClamped = clamp(i, minInteger, maxInteger);
		const real64 dither = randomReal64();

		return minReal + (((real64)(iClamped - minInteger) + dither) / ((real64)deltaInteger + 1)) * deltaReal;
	}

	inline integer quantize(real64 r,
						  real64 minReal, real64 maxReal,
						  integer minInteger, integer maxInteger)
	{
		const integer deltaInteger = maxInteger - minInteger;
		const real64 deltaReal = maxReal - minReal;

		return (integer)clamp(std::floor(minInteger + 
			((r - minReal) / deltaReal) * (deltaInteger + 1)), minInteger, maxInteger);
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

	template <int N>
	inline real64 ditheredQuantizeSigned(integer i)
	{
		BOOST_STATIC_ASSERT(N >= 2 && N <= 32);

		enum
		{
			Half = (1 << (N - 1)) - 1
		};
		
		real64 dither = randomReal64();
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

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits < FromBits), Integer>::type
		scaleInteger(const Integer& number)
	{
		BOOST_STATIC_ASSERT(FromBits > 0);
		BOOST_STATIC_ASSERT(ToBits > 0);

		enum
		{
			DeltaBits = FromBits - ToBits
		};

		return (number >> DeltaBits);
	}

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits == FromBits), Integer>::type
		scaleInteger(const Integer& number)
	{
		return number;
	}

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits > FromBits && ToBits <= 2 * FromBits), Integer>::type
		scaleInteger(const Integer& number)
	{
		BOOST_STATIC_ASSERT(FromBits > 0);
		BOOST_STATIC_ASSERT(ToBits > 0);

		enum
		{
			DeltaBits = ToBits - FromBits
		};

		BOOST_STATIC_ASSERT(DeltaBits >= 0);

		return (number << DeltaBits) +
			(number >> (FromBits - DeltaBits));
	}

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits > 2 * FromBits && ToBits <= 3 * FromBits), Integer>::type
		scaleInteger(const Integer& number)
	{
		BOOST_STATIC_ASSERT(FromBits > 0);
		BOOST_STATIC_ASSERT(ToBits > 0);

		enum
		{
			DeltaBits = ToBits - FromBits
		};

		BOOST_STATIC_ASSERT(DeltaBits >= 0);

		return (number << DeltaBits) +
			(number << (DeltaBits - FromBits)) +
			(number >> (2 * FromBits - DeltaBits));
	}

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits > 3 * FromBits && ToBits <= 4 * FromBits), Integer>::type
		scaleInteger(const Integer& number)
	{
		BOOST_STATIC_ASSERT(FromBits > 0);
		BOOST_STATIC_ASSERT(ToBits > 0);

		enum
		{
			DeltaBits = ToBits - FromBits
		};

		BOOST_STATIC_ASSERT(DeltaBits >= 0);

		return (number << DeltaBits) +
			(number << (DeltaBits - FromBits)) +
			(number << (DeltaBits - 2 * FromBits)) +
			(number >> (3 * FromBits - DeltaBits)) +;
	}

	template <int FromBit, int Bits, typename Integer>
	Integer extractBits(const Integer& number)
	{
		BOOST_STATIC_ASSERT(FromBit >= 0);
		BOOST_STATIC_ASSERT(Bits > 0);

		return (number >> FromBit) & ((1 << Bits) - 1);
	}

}

#endif
