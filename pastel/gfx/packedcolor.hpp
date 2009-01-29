#ifndef PASTELGFX_PACKEDCOLOR_HPP
#define PASTELGFX_PACKEDCOLOR_HPP

#include "pastel/gfx/packedcolor.h"

namespace Pastel
{

	inline uint32 colorToX8R8G8B8(const Color& color)
	{
		const uint32 red = (uint32)(color[0] * 255);
		const uint32 green = (uint32)(color[1] * 255);
		const uint32 blue = (uint32)(color[2] * 255);

		return (red << 16) + (green << 8) + blue;
	}

	inline Color colorFromX8R8G8B8(uint32 packedColor)
	{
		return Color(
			(real32)((packedColor >> 16) & 0xFF) / 255,
			(real32)((packedColor >> 8) & 0xFF) / 255,
			(real32)((packedColor >> 0) & 0xFF) / 255);
	}

	inline uint32 colorToR5G6B5(const Color& color)
	{
		const uint32 red = (uint32)(color[0] * 31);
		const uint32 green = (uint32)(color[1] * 63);
		const uint32 blue = (uint32)(color[2] * 31);

		return (red << 11) + (green << 5) + blue;
	}

}

#endif
