#ifndef PASTELGFX_COLOR_LAB_HPP
#define PASTELGFX_COLOR_LAB_HPP

#include "pastel/gfx/color_lab.h"
#include "pastel/gfx/color_illuminant.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	namespace LabDetail
	{

		inline real32 labFunction(real32 t)
		{
			static PASTEL_CONSTEXPR real32 OneThird = real32(1) / 3;
			static PASTEL_CONSTEXPR real32 TwoThird = real32(2) / 3;
			static PASTEL_CONSTEXPR real32 T0 = real32(216) / 24389;
			static PASTEL_CONSTEXPR real32 A = inverse(3 * std::pow(T0, TwoThird));
			static PASTEL_CONSTEXPR real32 B = real32(16) / 116;

			if (t > T0)
			{
				return std::pow(t, OneThird);
			}

			return A * t + B;
		}

		inline real32 labFunctionInverse(real32 x)
		{
			static PASTEL_CONSTEXPR real32 TwoThird = real32(2) / 3;
			static PASTEL_CONSTEXPR real32 T0 = real32(216) / 24389;
			static PASTEL_CONSTEXPR real32 B = real32(16) / 116;

			static PASTEL_CONSTEXPR real32 AInv = 3 * std::pow(T0, TwoThird);
			static PASTEL_CONSTEXPR real32 X0 = real32(6) / 29;
			static PASTEL_CONSTEXPR real32 Three = 3;

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
		static PASTEL_CONSTEXPR Color WhitePointInv(
			inverse(xyzIlluminantD65()));
		static PASTEL_CONSTEXPR real32 A = 116;
		static PASTEL_CONSTEXPR real32 B = 500;
		static PASTEL_CONSTEXPR real32 C = 200;
		static PASTEL_CONSTEXPR real32 D = 16;

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
		static PASTEL_CONSTEXPR Color WhitePoint(xyzIlluminantD65());
		static PASTEL_CONSTEXPR real32 AInv = inverse(real32(116));
		static PASTEL_CONSTEXPR real32 BInv = inverse(real32(500));
		static PASTEL_CONSTEXPR real32 CInv = inverse(real32(200));
		static PASTEL_CONSTEXPR real32 D = 16;

		const real32 fy((lab[0] + D) * AInv);
		const real32 fx(fy + (lab[1] * BInv));
		const real32 fz(fy - (lab[2] * CInv));

		return Color(
			LabDetail::labFunctionInverse(fx) * WhitePoint[0],
			LabDetail::labFunctionInverse(fy) * WhitePoint[1],
			LabDetail::labFunctionInverse(fz) * WhitePoint[2]);
	}

}

#endif
