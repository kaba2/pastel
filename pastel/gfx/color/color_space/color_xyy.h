// Description: CIE-xyY color space
// Detail: Chromaticity coordinates (x, y) + luminance Y
// Documentation: color_space.txt

#ifndef PASTELGFX_COLOR_XYY_H
#define PASTELGFX_COLOR_XYY_H

#include "pastel/gfx/color/color.h"

namespace Pastel
{

	//! Converts xyY color to XYZ color.
	Color xyyToXyz(const Color& xyy);

	//! Converts XYZ color to xyY color.
	Color xyzToXyy(const Color& xyz);

}

#include "pastel/gfx/color/color_space/color_xyy.hpp"

#endif
