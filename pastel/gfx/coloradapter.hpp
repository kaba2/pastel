#ifndef PASTELGFX_COLORADAPTER_HPP
#define PASTELGFX_COLORADAPTER_HPP

#include "pastel/gfx/coloradapter.h"

namespace Pastel
{

	template <typename Integer>
	Color integerToColor(
		const PASTEL_NO_DEDUCTION(Integer)& packedColor,
		integer redBits, integer greenBits, integer blueBits,
		integer redFrom, integer greenFrom, integer blueFrom)
	{
		PENSURE_OP(redBits, >, 0);
		PENSURE_OP(greenBits, >, 0);
		PENSURE_OP(blueBits, >, 0);

		PENSURE_OP(redFrom, >, 0);
		PENSURE_OP(greenFrom, >, 0);
		PENSURE_OP(blueFrom, >, 0);

		return Color(
			dequantizeUnsigned(extractBits(packedColor, redFrom, redBits), 1 << redBits),
			dequantizeUnsigned(extractBits(packedColor, greenFrom, greenBits), 1 << greenBits),
			dequantizeUnsigned(extractBits(packedColor, blueFrom, blueBits), 1 << blueBits));
	}

	template <typename Integer>
	Color integerToColor(
		const PASTEL_NO_DEDUCTION(Integer)& packedColor,
		integer redBits, integer greenBits, integer blueBits)
	{
		PENSURE_OP(redBits, >, 0);
		PENSURE_OP(greenBits, >, 0);
		PENSURE_OP(blueBits, >, 0);

		const integer redFrom = 0;
		const integer greenFrom = redFrom + redBits;
		const integer blueFrom = greenFrom + greenBits;

		return Pastel::integerToColor(
			packedColor, redBits, greenBits, blueBits,
			redFrom, greenFrom, blueFrom);
	}

	template <typename Integer>
	Integer colorToInteger(const Color& color,
		integer redBits, integer greenBits, integer blueBits,
		integer redFrom, integer greenFrom, integer blueFrom)
	{
		const Integer red = quantizeUnsigned(color[0], 1 << redBits);
		const Integer green = quantizeUnsigned(color[1], 1 << greenBits);
		const Integer blue = quantizeUnsigned(color[2], 1 << blueBits);

		return (red << redFrom) + (green << greenFrom) + (blue << blueFrom);
	}

	template <typename Integer>
	Integer colorToInteger(const Color& color,
		integer redBits, integer greenBits, integer blueBits)
	{
		const integer redFrom = 0;
		const integer greenFrom = redFrom + redBits;
		const integer blueFrom = greenFrom + greenBits;

		return Pastel::colorToInteger<Integer>(color,
			redBits, greenBits, blueBits,
			redFrom, greenFrom, blueFrom);
	}

}

#endif
