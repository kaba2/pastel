#ifndef PASTELGFX_COLOR_ILLUMINANT_HPP
#define PASTELGFX_COLOR_ILLUMINANT_HPP

#include "pastel/gfx/color_illuminant.h"
#include "pastel/gfx/color_space.h"
#include "pastel/gfx/color_xyy.h"

namespace Pastel
{

	// Standard illuminants (CIE 1931)

	inline Color xyzIlluminantE()
	{
		static PASTEL_CONSTEXPR Color color(
			xyyToXyz(Color(
			(real32)1 / 3, (real32)1 / 3, 1)));

		return color;
	}

	inline Color xyzIlluminantD50()
	{
		static PASTEL_CONSTEXPR Color color(
			xyyToXyz(Color(
			0.34567, 0.35850, 1)));

		return color;
	}

	inline Color xyzIlluminantD65()
	{
		static PASTEL_CONSTEXPR Color color(
			xyyToXyz(Color(
			0.31271, 0.32902, 1)));

		return color;
	}

}

#endif
