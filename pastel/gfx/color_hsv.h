// Description: HSV color space
// Detail: Hue-Saturation-Value
// Documentation: color_space.txt

#ifndef PASTELGFX_COLOR_HSV_H
#define PASTELGFX_COLOR_HSV_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

namespace Pastel
{

	//! Converts sRGB color to HSV color.
	PASTELGFX Color rgbToHsv(const Color& rgb);

	//! Converts HSV color to sRGB color.
	PASTELGFX Color hsvToRgb(const Color& hsv);

}

#endif
