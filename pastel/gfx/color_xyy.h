// Description: CIE-xyY color space
// Detail: Chromaticity coordinates (x, y) + luminance Y
// Documentation: color_space.txt

#ifndef PASTELGFX_COLOR_XYY_H
#define PASTELGFX_COLOR_XYY_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

namespace Pastel
{

	//! Converts xyY color to XYZ color.
	PASTELGFX Color xyyToXyz(const Color& xyy);

	//! Converts XYZ color to xyY color.
	PASTELGFX Color xyzToXyy(const Color& xyz);

}

#endif
