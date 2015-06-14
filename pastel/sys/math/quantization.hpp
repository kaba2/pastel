#ifndef PASTELSYS_QUANTIZATION_HPP
#define PASTELSYS_QUANTIZATION_HPP

#include "pastel/sys/math/quantization.h"

namespace Pastel
{

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

	template <integer N>
	inline real64 dequantizeSigned(integer i)
	{
		PASTEL_STATIC_ASSERT(N >= 2 && N <= 32);

		static constexpr int Half = (1 << (N - 1)) - 1;
		
		return clamp((real64)i / Half, -1, 1);
	}

	/*
	template <integer N>
	inline real64 ditheredQuantizeSigned(integer i)
	{
		PASTEL_STATIC_ASSERT(N >= 2 && N <= 32);

		static constexpr int Half = (1 << (N - 1)) - 1;
		
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

	template <integer N>
	inline integer quantizeSigned(real64 r)
	{
		PASTEL_STATIC_ASSERT(N >= 2 && N <= 32);

		static constexpr int Half = (1 << (N - 1)) - 1;

		return (integer)clamp(std::floor(r * Half + (r + 1) / 2), -Half, Half);
	}

}

#endif
