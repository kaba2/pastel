#ifndef PASTELGFX_COLOR_TOOLS_HPP
#define PASTELGFX_COLOR_TOOLS_HPP

#include "pastel/gfx/color/color_tools.h"

#include "pastel/sys/math/constants.h"
#include "pastel/sys/math_functions.h"
#include "pastel/sys/random/random_uniform.h"
#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	inline Color randomRgbColor()
	{
		return Color(random<real32>(), random<real32>(), random<real32>());
	}

	// Grayscale conversion

	inline Color lumaWeights()
	{
		return Color(0.299, 0.587, 0.114);
	}

	inline real32 luma(const Color& rgb)
	{
		return dot(rgb, lumaWeights());
	}

	// Gamut compression

	inline Color fitColorBlack(const Color& rgb)
	{
		if (anyLess(rgb, 0))
		{
			return Color(0);
		}

		real32 maxComponent = max(rgb);

		if (maxComponent > 1)
		{
			// If the max component is greater
			// than 1, scale the rgb such
			// that max component becomes 1.

			return rgb / maxComponent;
		}

		return rgb;
	}

	inline Color fitColor(const Color& rgb)
	{
		Color clampedZero = max(rgb, 0);
		real32 maxComponent = max(clampedZero);

		if (maxComponent > 1)
		{
			// If the max component is greater
			// than 1, scale the rgb such
			// that max component becomes 1.

			return clampedZero / maxComponent;
		}

		return clampedZero;
	}

	inline Color fitNegativeColor(const Color& rgb)
	{
		return max(rgb, 0);
	}

}

#endif
