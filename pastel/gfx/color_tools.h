#ifndef PASTELGFX_COLORTOOLS_H
#define PASTELGFX_COLORTOOLS_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Generates a random rgb color.

	Color randomRgbColor();

	// Standard illuminants (CIE 1964)

	//! Returns the XYZ coordinates of CIE illuminant E.

	Color xyzIlluminantE();

	//! Returns the XYZ coordinates of CIE illuminant D50.

	Color xyzIlluminantD50();

	//! Returns the XYZ coordinates of CIE illuminant D65.

	Color xyzIlluminantD65();



	// Gamma conversion

	//! Converts from linear-sRGB to gamma-sRGB.

	Color linearSrgbToGammaSrgb(const Color& rgb);

	//! Converts from gamma-sRGB to linear-sRGB.

	Color gammaSrgbToLinearSrgb(const Color& rgb);



	// Grayscale conversion

	//! Returns the weights of the rgb-to-luma conversion.

	Color rgbLumaWeights();

	//! Computes the luma of an rgb color.
	/*!
	The luma is computed from non-linear sRGB values
	and is a crude approximation to lightness. An ideal
	conversion would convert to CIELab, zero the ab components
	and convert back to sRGB. This is quite costly an
	operation and for many applications luma does good enough.
	*/

	real32 rgbLuma(const Color& rgb);



	// Gamut compression

	//! Sets all colors out of the [0, 1] range to black.

	Color fitColorBlack(const Color& rgb);

	//! Fits an rgb color to [0, 1]^3 range while preserving hue.
	/*!
	Components less than zero are set to zero.
	If a color has a component greater than 1, then
	the color is normalized by max(rgb). This operation
	maps the color to the legal range by decreasing intensity
	while preserving hue.
	If no such cases apply, the color is returned as it is.
	*/

	Color fitColor(const Color& rgb);

	//! Sets negative components to zero.

	Color fitNegativeColor(const Color& rgb);

	// Color conversion

	//! Converts sRGB color to HSV color.
	/*!
	The HSV color coordinates will be in
	the range [0, 2pi[ x [0, 1]^2.
	*/

	Color rgbToHsv(const Color& rgb);

	//! Converts HSV color to sRGB color.
	/*!
	The HSV color coordinates will be in
	the range [0, 2pi[ x [0, 1]^2.
	*/

	Color hsvToRgb(const Color& rgb);

	//! Computes the linear-sRGB to XYZ transformation matrix.
	/*!
	This is computed by using the 'linearRgbToXyzTransform'
	function. The sRGB standard chooses the chromaticity
	coordinates for the light emitters as well as for the
	white point.
	*/

	Matrix<3, 3, real32> linearSrgbToXyzTransform();

	Matrix<3, 3, real32> xyzToLinearSrgbTransform();

	//! Computes a linear-RGB to XYZ transformation matrix.

	Matrix<3, 3, real32> linearRgbToXyzTransform(
		const Color& xyzRed,
		const Color& xyzGreen,
		const Color& xyzBlue,
		const Color& xyzWhite);

	//! Converts XYZ color to sRGB color.

	Color xyzToSrgb(const Color& xyz);

	//! Converts sRGB color to XYZ color.

	Color srgbToXyz(const Color& rgb);

	//! Converts XYZ color to CIELab color

	Color xyzToLab(const Color& xyz);

	//! Converts CIELab color to XYZ color

	Color labToXyz(const Color& lab);

	//! Converts XYy color to XYZ color.

	Color xyyToXyz(const Color& xyy);

	//! Converts XYZ color to XYy color.

	Color xyzToXyy(const Color& xyz);

	//! Converts CIELab color to CIELch color.

	Color labToLch(const Color& lab);

	//! Converts CIELch color to CIELab color.

	Color lchToLab(const Color& lch);

	//! Converts XYZ color LMS color.

	Color xyzToLms(const Color& xyz);

	//! Converts LMS color XYZ color.

	Color lmsToXyz(const Color& lms);

}

#include "pastel/gfx/color_tools.hpp"

#endif
