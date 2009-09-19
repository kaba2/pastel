#include "pastel/gfx/color_srgb.h"
#include "pastel/gfx/color_xyy.h"
#include "pastel/gfx/color_illuminant.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/math_functions.h"

#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	PASTELGFX Color linearSrgbToSrgb(const Color& rgb)
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

	PASTELGFX Color srgbToLinearSrgb(const Color& rgb)
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

	PASTELGFX Matrix<real32, 3, 3> linearSrgbToXyzTransform()
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

	PASTELGFX Matrix<real32, 3, 3> xyzToLinearSrgbTransform()
	{
		static const Matrix<real32, 3, 3> Conversion(
			inverse(linearSrgbToXyzTransform()));

		return Conversion;
	}

	PASTELGFX Matrix<real32, 3, 3> linearRgbToXyzTransform(
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

		const Matrix<real32, 3, 3> primaryMatrix(
			xyzRed,
			xyzGreen,
			xyzBlue);

		const Color primaryWeights(
			solveLinear(primaryMatrix, xyzWhite));

		// Use the weights to form the final transformation
		// matrix.

		return Matrix<real32, 3, 3>(
			xyzRed * primaryWeights[0],
			xyzGreen * primaryWeights[1],
			xyzBlue * primaryWeights[2]);
	}

	PASTELGFX Color xyzToSrgb(const Color& xyz)
	{
		static const Matrix<real32, 3, 3> Conversion(
			inverse(linearSrgbToXyzTransform()));

		return linearSrgbToSrgb(fitNegativeColor(xyz * Conversion));
	}

	PASTELGFX Color srgbToXyz(const Color& rgb)
	{
		static const Matrix<real32, 3, 3> Conversion(
			linearSrgbToXyzTransform());

		return srgbToLinearSrgb(rgb) * Conversion;
	}

}
