// Description: YUV color space
// Documentation: color_space.txt

#ifndef PASTEL_COLOR_YUV_H
#define PASTEL_COLOR_YUV_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

namespace Pastel
{

	//! Converts sRGB color to YUV color.
	PASTELGFX Color rgbToYuv(const Color& rgb);

	//! Converts YUV color to sRGB color.
	PASTELGFX Color yuvToRgb(const Color& yuv);

}

#endif
