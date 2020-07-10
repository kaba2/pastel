// Description: Standard illuminants

#ifndef PASTELGFX_COLOR_ILLUMINANT_H
#define PASTELGFX_COLOR_ILLUMINANT_H

#include "pastel/gfx/color/color.h"
#include "pastel/gfx/color/color_space.h"
#include "pastel/gfx/color/color_space/color_xyy.h"

namespace Pastel
{

	// Standard illuminants (CIE 1931)

	//! Returns the XYZ coordinates of CIE illuminant E.
	inline Color xyzIlluminantE()
	{
		static const Color color(
			xyyToXyz(Color(
			(real32)1 / 3, (real32)1 / 3, 1)));

		return color;
	}

	//! Returns the XYZ coordinates of CIE illuminant D50.
	inline Color xyzIlluminantD50()
	{
		static const Color color(
			xyyToXyz(Color(
			0.34567, 0.35850, 1)));

		return color;
	}

	//! Returns the XYZ coordinates of CIE illuminant D65.
	inline Color xyzIlluminantD65()
	{
		static const Color color(
			xyyToXyz(Color(
			0.31271, 0.32902, 1)));

		return color;
	}

}

#endif
