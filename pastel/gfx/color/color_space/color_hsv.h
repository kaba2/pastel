// Description: HSV color space
// Detail: Hue-Saturation-Value
// Documentation: color_space.txt

#ifndef PASTELGFX_COLOR_HSV_H
#define PASTELGFX_COLOR_HSV_H

#include "pastel/gfx/color.h"

namespace Pastel
{

	//! Converts sRGB color to HSV color.
	Color rgbToHsv(const Color& rgb);

	//! Converts HSV color to sRGB color.
	Color hsvToRgb(const Color& hsv);

}

#include "pastel/gfx/color_hsv.hpp"

#endif
