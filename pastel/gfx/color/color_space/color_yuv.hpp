#ifndef PASTELGFX_COLOR_YUV_HPP
#define PASTELGFX_COLOR_YUV_HPP

#include "pastel/gfx/color/color_yuv.h"

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Converts sRGB color to YUV color.
	inline Color rgbToYuv(const Color& rgb)
	{
		const static Matrix<real32, 3, 3> conversion =
			matrix3x3<real32>(
			0.299, 0.587, 0.114,
			-0.1687, -0.3313, 0.5,
			0.5, -0.4187, -0.0813);

		return conversion * rgb;
	}

	//! Converts YUV color to sRGB color.
	inline Color yuvToRgb(const Color& yuv)
	{
		const static Matrix<real32, 3, 3> conversion =
			matrix3x3<real32>(
			1, 0, 1.402,
			1, -0.34414, -0.71414,
			1, 1.772, 0);

		return conversion * yuv;
	}

}

#endif
