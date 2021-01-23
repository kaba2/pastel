// Description: sRGB color space
// Detail: Red-Green-Blue
// Documentation: color_space.txt

#ifndef PASTELGFX_COLOR_SRGB_H
#define PASTELGFX_COLOR_SRGB_H

#include "pastel/gfx/color/color.h"

#include "pastel/gfx/color/color_space/color_xyy.h"
#include "pastel/gfx/color/color_illuminant.h"
#include "pastel/gfx/color/color_tools.h"

#include "pastel/sys/math_functions.h"

#include "pastel/math/matrix/matrix.h"
#include "pastel/math/matrix/matrix_inverse.h"
#include "pastel/math/matrix/lu_decomposition.h"

namespace Pastel
{

	//! Converts from linear-sRGB to sRGB.
	inline Color linearSrgbToSrgb(const Color& rgb)
	{
		Color result;

		for (integer i = 0;i < 3;++i)
		{
			if (rgb[i] < (real32)0.00304)
			{
				result[i] = rgb[i] * (real32)12.92;
			}
			else
			{
				result[i] = std::pow(rgb[i] * (real32)1.055,
					(real32)1 / (real32)2.4) - (real32)0.055;
			}
		}

		return result;
	}

	//! Converts from sRGB to linear-sRGB.
	inline Color srgbToLinearSrgb(const Color& rgb)
	{
		Color result;

		for (integer i = 0;i < 3;++i)
		{
			if (rgb[i] <= (real32)0.03928)
			{
				result[i] = rgb[i] / (real32)12.92;
			}
			else
			{
				result[i] = std::pow((rgb[i] + (real32)0.055) /
					(real32)1.055, (real32)2.4);
			}
		}

		return result;
	}

	//! Computes a linear-RGB to XYZ transformation matrix.
	inline Matrix<real32, 3, 3> linearRgbToXyzTransform(
		const Color& xyzRed,
		const Color& xyzGreen,
		const Color& xyzBlue,
		const Color& xyzWhite)
	{
		// [1, 0, 0] must map to (xyzRed * r)
		// [0, 1, 0] must map to (xyzGreen * g)
		// [0, 0, 1] must map to (xyzBlue * b)
		// [1, 1, 1] must map to xyzWhite

		// From the first three conditions we get the
		// equation:
		// [R, G, B] [xyzRed * r]   = [X, Y, Z]
		//           [xyzGreen * g]
		//           [xyzBlue * b]
		//
		// To solve for [r, g, b], we use the fourth
		// condition:
		// [1, 1, 1] [xyzRed * r]   = xyzWhite
		//           [xyzGreen * g]
		//           [xyzBlue * b]
		//
		// This can be written as:
		// [r, g, b] [xyzRed]   = xyzWhite
		//           [xyzGreen]
		//           [xyzBlue]
		//
		// Which is a standard linear equation system.

		Matrix<real32, 3, 3> primaryMatrix =
			matrix3x3<real32>(xyzRed, xyzGreen, xyzBlue);

		Color primaryWeights(
			solveLinearInplace(view(primaryMatrix), xyzWhite));

		// Use the weights to form the final transformation
		// matrix.

		return matrix3x3<real32>(
			evaluate(xyzRed * primaryWeights[0]),
			evaluate(xyzGreen * primaryWeights[1]),
			evaluate(xyzBlue * primaryWeights[2]));
	}

	//! Returns the linear-sRGB to XYZ transformation matrix.
	/*!
	This is computed by using the 'linearRgbToXyzTransform'
	function. The sRGB standard chooses the chromaticity
	coordinates for the light emitters as well as for the
	white point.
	*/
	inline Matrix<real32, 3, 3> linearSrgbToXyzTransform()
	{
		// The sRGB standard chooses standard
		// light emitters by specifying their
		// chromaticy coordinates (with luminance 1).
		// These are given below.

		static const Color xyzRed(
			xyyToXyz(Color(0.6400, 0.3300, 1)));
		static const Color xyzGreen(
			xyyToXyz(Color(0.3000, 0.6000, 1)));
		static const Color xyzBlue(
			xyyToXyz(Color(0.1500, 0.0600, 1)));

		// The sRGB standard specifies the white
		// point to be the CIE standard illuminant D65.

		static const Color xyzWhite(
			xyzIlluminantD65());

		static const Matrix<real32, 3, 3> transformation(
			linearRgbToXyzTransform(
			xyzRed, xyzGreen, xyzBlue,
			xyzWhite));

		return transformation;
	}

	//! Returns the XYZ to linear-sRGB transformation matrix.
	/*!
	This is the inverse matrix of the linearSrgbToXyzTransform()
	matrix.
	*/
	inline Matrix<real32, 3, 3> xyzToLinearSrgbTransform()
	{
		static const Matrix<real32, 3, 3> Conversion(
			inverse(linearSrgbToXyzTransform()));

		return Conversion;
	}

	//! Converts XYZ color to sRGB color.
	inline Color xyzToSrgb(const Color& xyz)
	{
		static const Matrix<real32, 3, 3> Conversion(
			inverse(linearSrgbToXyzTransform()));

		return linearSrgbToSrgb(fitNegativeColor(xyz * Conversion));
	}

	//! Converts sRGB color to XYZ color.
	inline Color srgbToXyz(const Color& rgb)
	{
		static const Matrix<real32, 3, 3> Conversion(
			linearSrgbToXyzTransform());

		return srgbToLinearSrgb(rgb) * Conversion;
	}

}

#endif
