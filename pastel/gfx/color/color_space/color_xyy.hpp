#ifndef PASTELGFX_COLOR_XYY_HPP
#define PASTELGFX_COLOR_XYY_HPP

#include "pastel/gfx/color/color_space/color_xyy.h"

namespace Pastel
{

	//! Converts xyY color to XYZ color.
	inline Color xyyToXyz(const Color& xyy)
	{
		// If y != 0, then
		// X = (Y / y)x
		// Z = (Y / y)(1 - x - y)
		//
		// Proof:
		// Y / y = Y / (Y / (X + Y + Z))
		// = X + Y + Z
		//
		// (Y / y)x = (X + Y + Z)x
		// = (X + Y + Z)(X / (X + Y + Z)) = X
		//
		// (Y / y)(1 - x - y)
		// = (X + Y + Z)(1 - x - y)
		// = (X + Y + Z)(1 - (X + Y) / (X + Y + Z))
		// = (X + Y + Z)(Z / (X + Y + Z))
		// = Z

		real32 yRatio(xyy[2] / xyy[1]);
		return Color(
			yRatio * xyy[0],
			xyy[2],
			yRatio * (1 - xyy[0] - xyy[1]));
	}

	//! Converts XYZ color to xyY color.
	inline Color xyzToXyy(const Color& xyz)
	{
		// x' = X / (X + Y + Z)
		// y' = Y / (X + Y + Z)
		// Y' = Y

		real32 s = sum(xyz);

		return Color(
			xyz[0] / s,
			xyz[1] / s,
			xyz[1]);
	}

}

#endif
