#include "pastel/gfx/color_lab.h"
#include "pastel/gfx/color_illuminant.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	namespace LabDetail
	{

		PASTELGFX real32 labFunction(real32 t)
		{
			PASTEL_CONSTEXPR real32 OneThird = real32(1) / 3;
			PASTEL_CONSTEXPR real32 TwoThird = real32(2) / 3;
			PASTEL_CONSTEXPR real32 T0 = real32(216) / 24389;
			PASTEL_CONSTEXPR real32 A = inverse(3 * std::pow(T0, TwoThird));
			PASTEL_CONSTEXPR real32 B = real32(16) / 116;

			if (t > T0)
			{
				return std::pow(t, OneThird);
			}

			return A * t + B;
		}

		PASTELGFX real32 labFunctionInverse(real32 x)
		{
			PASTEL_CONSTEXPR real32 TwoThird = real32(2) / 3;
			PASTEL_CONSTEXPR real32 T0 = real32(216) / 24389;
			PASTEL_CONSTEXPR real32 B = real32(16) / 116;

			PASTEL_CONSTEXPR real32 AInv = 3 * std::pow(T0, TwoThird);
			PASTEL_CONSTEXPR real32 X0 = real32(6) / 29;
			PASTEL_CONSTEXPR real32 Three = 3;

			if (x > X0)
			{
				return std::pow(x, Three);
			}

			return (x - B) * AInv;
		}

	}

	PASTELGFX Color xyzToLab(const Color& xyz)
	{
		// CIE Illuminant D65
		PASTEL_CONSTEXPR Color WhitePointInv(
			inverse(xyzIlluminantD65()));
		PASTEL_CONSTEXPR real32 A = 116;
		PASTEL_CONSTEXPR real32 B = 500;
		PASTEL_CONSTEXPR real32 C = 200;
		PASTEL_CONSTEXPR real32 D = 16;

		const real32 fx(LabDetail::labFunction(xyz[0] * WhitePointInv[0]));
		const real32 fy(LabDetail::labFunction(xyz[1] * WhitePointInv[1]));
		const real32 fz(LabDetail::labFunction(xyz[2] * WhitePointInv[2]));

		return Color(
			A * fy - D,
			B * (fx - fy),
			C * (fy - fz));
	}

	PASTELGFX Color labToXyz(const Color& lab)
	{
		// CIE Illuminant D65
		PASTEL_CONSTEXPR Color WhitePoint(xyzIlluminantD65());
		PASTEL_CONSTEXPR real32 AInv = inverse(real32(116));
		PASTEL_CONSTEXPR real32 BInv = inverse(real32(500));
		PASTEL_CONSTEXPR real32 CInv = inverse(real32(200));
		PASTEL_CONSTEXPR real32 D = 16;

		const real32 fy((lab[0] + D) * AInv);
		const real32 fx(fy + (lab[1] * BInv));
		const real32 fz(fy - (lab[2] * CInv));

		return Color(
			LabDetail::labFunctionInverse(fx) * WhitePoint[0],
			LabDetail::labFunctionInverse(fy) * WhitePoint[1],
			LabDetail::labFunctionInverse(fz) * WhitePoint[2]);
	}

}
