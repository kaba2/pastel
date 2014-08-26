// Description: YUV color space
// Documentation: color_space.txt

#ifndef PASTELGFX_COLOR_YUV_H
#define PASTELGFX_COLOR_YUV_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

namespace Pastel
{

	//! Converts sRGB color to YUV color.
	Color rgbToYuv(const Color& rgb);

	//! Converts YUV color to sRGB color.
	Color yuvToRgb(const Color& yuv);

}

#include "pastel/gfx/color_yuv.hpp"

#endif
