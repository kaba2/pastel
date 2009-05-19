#ifndef PASTEL_COLORTOOLS_HPP
#define PASTEL_COLORTOOLS_HPP

#include "pastel/gfx/color_tools.h"

#include "pastel/sys/tuple_tools.h"
#include "pastel/sys/random.h"

#include "pastel/math/matrix.h"
#include "pastel/math/matrix_tools.h"
#include "pastel/sys/vector.h"
#include "pastel/math/coordinates.h"

#include "pastel/sys/constants.h"
#include "pastel/sys/mathfunctions.h"
#include "pastel/math/minmax.h"

namespace Pastel
{

	inline Color randomRgbColor()
	{
		return Color(random<real32>(), random<real32>(), random<real32>());
	}

	// Standard illuminants (CIE 1931)

	inline Color xyzIlluminantE()
	{
		static const Color color(
			xyyToXyz(Color(
			(real32)1 / 3, (real32)1 / 3, 1)));

		return color;
	}

	inline Color xyzIlluminantD50()
	{
		static const Color color(
			xyyToXyz(Color(
			0.34567, 0.35850, 1)));

		return color;
	}

	inline Color xyzIlluminantD65()
	{
		static const Color color(
			xyyToXyz(Color(
			0.31271, 0.32902, 1)));

		return color;
	}



	// Gamma conversion

	inline Color linearSrgbToGammaSrgb(const Color& rgb)
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

	inline Color gammaSrgbToLinearSrgb(const Color& rgb)
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



	// Grayscale conversion

	inline Color lumaWeights()
	{
		return Color(0.299, 0.587, 0.114);
	}

	inline real32 luma(const Color& rgb)
	{
		return dot(rgb, lumaWeights());
	}

	// Gamut compression

	inline Color fitColorBlack(const Color& rgb)
	{
		if (anyLess(rgb, 0))
		{
			return Color(0);
		}

		const real32 maxComponent = max(rgb);

		if (maxComponent > 1)
		{
			// If the max component is greater
			// than 1, scale the rgb such
			// that max component becomes 1.

			return rgb / maxComponent;
		}

		return rgb;
	}

	inline Color fitColor(const Color& rgb)
	{
		const Color clampedZero = max(rgb, 0);
		const real32 maxComponent = max(clampedZero);

		if (maxComponent > 1)
		{
			// If the max component is greater
			// than 1, scale the rgb such
			// that max component becomes 1.

			return clampedZero / maxComponent;
		}

		return clampedZero;
	}

	inline Color fitNegativeColor(const Color& rgb)
	{
		return max(rgb, 0);
	}


	// Color conversion

	inline Color rgbToHsv(const Color& rgb)
	{
		Color result(0);

		const integer indexMin = minIndex(rgb);
		const integer indexMax = maxIndex(rgb);

		const real32 rgbMin = rgb[indexMin];
		const real32 rgbMax = rgb[indexMax];

		const real32 scale = (real32)1 / 6;

		const real32 delta = rgbMax - rgbMin;

		// Compute hue h.

		// EPSILON
		if (delta != 0)
		{
			if (indexMax == 0)
			{
				result[0] =  scale * ((rgb[1] - rgb[2]) / delta);
				if (result[0] < 0)
				{
					result[0] += 1;
				}
			}
			else if (indexMax == 1)
			{
				result[0] = scale * ((rgb[2] - rgb[0]) / delta) +
					scale * 2;
			}
			else
			{
				result[0] = scale * ((rgb[0] - rgb[1]) / delta) +
					scale * 4;
			}
		}
		else
		{
			result[0] = 0;
		}

		// Compute saturation s.

		// EPSILON
		if (rgbMax != 0)
		{
			result[1] = 1 - rgbMin / rgbMax;
		}
		else
		{
			result[1] = 0;
		}

		// Compute value v.

		result[2] = rgbMax;

		return result;
	}

	inline Color hsvToRgb(const Color& hsv)
	{
		const real32 g =
			6 * hsv[0];
		const integer gFloor = std::floor(g);

		const real32 f = g - gFloor;
		const real32 p = hsv[2] * (1 - hsv[1]);
		const real32 q = hsv[2] * (1 - f * hsv[1]);
		const real32 t = hsv[2] * (1 - (1 - f) * hsv[1]);

		const integer i = gFloor % 6;

		if (i == 0)
		{
			return Color(hsv[2], t, p);
		}
		else if (i == 1)
		{
			return Color(q, hsv[2], p);
		}
		else if (i == 2)
		{
			return Color(p, hsv[2], t);
		}
		else if (i == 3)
		{
			return Color(p, q, hsv[2]);
		}
		else if (i == 4)
		{
			return Color(t, p, hsv[2]);
		}

		return Color(hsv[2], p, q);
	}

	inline Matrix<3, 3, real32> linearSrgbToXyzTransform()
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

		static const Matrix<3, 3, real32> transformation(
			linearRgbToXyzTransform(
			xyzRed, xyzGreen, xyzBlue,
			xyzWhite));

		return transformation;
	}

	inline Matrix<3, 3, real32> xyzToLinearSrgbTransform()
	{
		static const Matrix<3, 3, real32> Conversion(
			inverse(linearSrgbToXyzTransform()));

		return Conversion;
	}

	inline Matrix<3, 3, real32> linearRgbToXyzTransform(
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

		const Matrix<3, 3, real32> primaryMatrix(
			xyzRed,
			xyzGreen,
			xyzBlue);

		const Color primaryWeights(
			solveLinearSystem(primaryMatrix, xyzWhite));

		// Use the weights to form the final transformation
		// matrix.

		return Matrix<3, 3, real32>(
			xyzRed * primaryWeights[0],
			xyzGreen * primaryWeights[1],
			xyzBlue * primaryWeights[2]);
	}

	inline Color xyzToSrgb(const Color& xyz)
	{
		static const Matrix<3, 3, real32> Conversion(
			inverse(linearSrgbToXyzTransform()));

		return linearSrgbToGammaSrgb(fitNegativeColor(xyz * Conversion));
	}

	inline Color srgbToXyz(const Color& rgb)
	{
		static const Matrix<3, 3, real32> Conversion(
			linearSrgbToXyzTransform());

		return gammaSrgbToLinearSrgb(rgb) * Conversion;
	}

	namespace LabDetail
	{

		inline real32 labFunction(real32 t)
		{
			static const real32 OneThird = real32(1) / 3;
			static const real32 TwoThird = real32(2) / 3;
			static const real32 T0 = real32(216) / 24389;
			static const real32 A = inverse(3 * std::pow(T0, TwoThird));
			static const real32 B = real32(16) / 116;

			if (t > T0)
			{
				return std::pow(t, OneThird);
			}

			return A * t + B;
		}

		inline real32 labFunctionInverse(real32 x)
		{
			static const real32 TwoThird = real32(2) / 3;
			static const real32 T0 = real32(216) / 24389;
			static const real32 B = real32(16) / 116;

			static const real32 AInv = 3 * std::pow(T0, TwoThird);
			static const real32 X0 = real32(6) / 29;
			static const real32 Three = 3;

			if (x > X0)
			{
				return std::pow(x, Three);
			}

			return (x - B) * AInv;
		}

	}

	inline Color xyzToLab(const Color& xyz)
	{
		// CIE Illuminant D65
		static const Color WhitePointInv(
			inverse(xyzIlluminantD65()));
		static const real32 A = 116;
		static const real32 B = 500;
		static const real32 C = 200;
		static const real32 D = 16;

		const real32 fx(LabDetail::labFunction(xyz[0] * WhitePointInv[0]));
		const real32 fy(LabDetail::labFunction(xyz[1] * WhitePointInv[1]));
		const real32 fz(LabDetail::labFunction(xyz[2] * WhitePointInv[2]));

		return Color(
			A * fy - D,
			B * (fx - fy),
			C * (fy - fz));
	}

	inline Color labToXyz(const Color& lab)
	{
		// CIE Illuminant D65
		static const Color WhitePoint(xyzIlluminantD65());
		static const real32 AInv = inverse(real32(116));
		static const real32 BInv = inverse(real32(500));
		static const real32 CInv = inverse(real32(200));
		static const real32 D = 16;

		const real32 fy((lab[0] + D) * AInv);
		const real32 fx(fy + (lab[1] * BInv));
		const real32 fz(fy - (lab[2] * CInv));

		return Color(
			LabDetail::labFunctionInverse(fx) * WhitePoint[0],
			LabDetail::labFunctionInverse(fy) * WhitePoint[1],
			LabDetail::labFunctionInverse(fz) * WhitePoint[2]);
	}

	inline Color xyyToXyz(const Color& xyy)
	{
		// If y != 0, then
		// X = (Y / y)x
		// Z = (Y / y)(1 - x - y)
		//
		// Proof:
		// Y / y = Y / (Y / (X + Y + Z))
		// = X + Y + Z
		//
		// (Y / y)x = (X + Y + Z)x
		// = (X + Y + Z)(X / (X + Y + Z)) = X
		//
		// (Y / y)(1 - x - y)
		// = (X + Y + Z)(1 - x - y)
		// = (X + Y + Z)(1 - (X + Y) / (X + Y + Z))
		// = (X + Y + Z)(Z / (X + Y + Z))
		// = Z

		const real32 yRatio(xyy[2] / xyy[1]);
		return Color(
			yRatio * xyy[0],
			xyy[2],
			yRatio * (1 - xyy[0] - xyy[1]));
	}

	inline Color xyzToXyy(const Color& xyz)
	{
		// x' = X / (X + Y + Z)
		// y' = Y / (X + Y + Z)
		// Y' = Y

		const real32 s = sum(xyz);

		return Color(
			xyz[0] / s,
			xyz[1] / s,
			xyz[1]);
	}

	inline Color labToLch(const Color& lab)
	{
		const Color shuffled = Color(lab[1], lab[2], lab[0]);

		const Color cylinder(cartesianToCylinder(shuffled));
		return Color(cylinder[2], cylinder[0], cylinder[1]);
	}

	inline Color lchToLab(const Color& lch)
	{
		const Color cylinder = Color(lch[1], lch[2], lch[0]);
		const Color shuffled(cylinderToCartesian(cylinder));

		return Color(shuffled[2], shuffled[0], shuffled[1]);
	}

	inline Matrix<3, 3, real32> xyzToLmsTransform()
	{
		static const Matrix<3, 3, real32> Conversion(
			 0.7328, -0.7036, 0.0030,
			 0.4296,  1.6975, 0.0136,
			-0.1624,  0.0061, 0.9834);

		return Conversion;
	}

	inline Matrix<3, 3, real32> lmsToXyzTransform()
	{
		static const Matrix<3, 3, real32> Conversion(
			inverse(xyzToLmsTransform()));

		return Conversion;
	}

	inline Color xyzToLms(const Color& xyz)
	{
		return xyz * xyzToLmsTransform();
	}

	inline Color lmsToXyz(const Color& lms)
	{
		return lms * lmsToXyzTransform();
	}

}

#endif
