#include "pastel/gfx/color_hsv.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	PASTELGFX Color rgbToHsv(const Color& rgb)
	{
		Color result(0);

		integer indexMin = minIndex(rgb);
		integer indexMax = maxIndex(rgb);

		const real32 rgbMin = rgb[indexMin];
		const real32 rgbMax = rgb[indexMax];

		const real32 scale = (real32)1 / 6;

		const real32 delta = rgbMax - rgbMin;

		// Compute hue h.

		// EPSILON
		if (delta != 0)
		{
			if (indexMax == 0)
			{

				result[0] =  scale * ((rgb[1] - rgb[2]) / delta);
				if (result[0] < 0)
				{
					result[0] += 1;
				}
			}
			else if (indexMax == 1)
			{
				result[0] = scale * ((rgb[2] - rgb[0]) / delta) +
					scale * 2;
			}
			else
			{
				result[0] = scale * ((rgb[0] - rgb[1]) / delta) +
					scale * 4;
			}
		}
		else
		{
			result[0] = 0;
		}

		// Compute saturation s.

		// EPSILON
		if (rgbMax != 0)
		{
			result[1] = 1 - rgbMin / rgbMax;
		}
		else
		{
			result[1] = 0;
		}

		// Compute value v.

		result[2] = rgbMax;

		return result;
	}

	PASTELGFX Color hsvToRgb(const Color& hsv)
	{
		real32 g =

			6 * hsv[0];
		integer gFloor = std::floor(g);

		real32 f = g - gFloor;

		const real32 p = hsv[2] * (1 - hsv[1]);
		const real32 q = hsv[2] * (1 - f * hsv[1]);
		const real32 t = hsv[2] * (1 - (1 - f) * hsv[1]);

		integer i = gFloor % 6;

		if (i == 0)
		{
			return Color(hsv[2], t, p);
		}
		else if (i == 1)
		{
			return Color(q, hsv[2], p);
		}
		else if (i == 2)
		{
			return Color(p, hsv[2], t);
		}
		else if (i == 3)
		{
			return Color(p, q, hsv[2]);
		}
		else if (i == 4)
		{
			return Color(t, p, hsv[2]);
		}

		return Color(hsv[2], p, q);
	}

}
