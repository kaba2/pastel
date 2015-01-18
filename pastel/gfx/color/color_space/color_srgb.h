// Description: sRGB color space
// Detail: Red-Green-Blue
// Documentation: color_space.txt

#ifndef PASTELGFX_COLOR_SRGB_H
#define PASTELGFX_COLOR_SRGB_H

#include "pastel/gfx/color/color.h"

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Converts from linear-sRGB to sRGB.
	Color linearSrgbToSrgb(const Color& rgb);

	//! Converts from sRGB to linear-sRGB.
	Color srgbToLinearSrgb(const Color& rgb);

	//! Returns the linear-sRGB to XYZ transformation matrix.
	/*!
	This is computed by using the 'linearRgbToXyzTransform'
	function. The sRGB standard chooses the chromaticity
	coordinates for the light emitters as well as for the
	white point.
	*/
	Matrix<real32> linearSrgbToXyzTransform();

	//! Returns the XYZ to linear-sRGB transformation matrix.
	/*!
	This is the inverse matrix of the linearSrgbToXyzTransform()
	matrix.
	*/
	Matrix<real32> xyzToLinearSrgbTransform();

	//! Computes a linear-RGB to XYZ transformation matrix.
	Matrix<real32> linearRgbToXyzTransform(
		const Color& xyzRed,
		const Color& xyzGreen,
		const Color& xyzBlue,
		const Color& xyzWhite);

	//! Converts XYZ color to sRGB color.
	Color xyzToSrgb(const Color& xyz);

	//! Converts sRGB color to XYZ color.
	Color srgbToXyz(const Color& rgb);

}

#include "pastel/gfx/color/color_space/color_srgb.hpp"

#endif
