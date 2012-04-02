#include "pastelmathtest.h"

#include "pastel/gfx/pcx.h"
#include "pastel/gfx/color_hsv.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/array.h"
#include <complex>

using namespace Pastel;

namespace
{

	void testSin()
	{
		const integer Width = 512;
		const integer Height = 512;

		Array<Color, 2> image(Vector2i(Width, Height));

		for (integer y = 0;y < Height;++y)
		{
			for (integer x = 0;x < Width;++x)
			{
				/*
				const std::complex<real> z(
					(real)(x - (Width / 2)) / (Width / 8),
					(real)(y - (Height / 2)) / (Height / 8));

				const std::complex<real> zSin = sin(z);

				image(x, y) = hsvToRgb(Color(arg(zSin), 1, mod(zSin)));
				*/
				const std::complex<real> z(
					(real)(x - (Width / 2)) / (Width / 8),
					(real)(y - (Height / 2)) / (Height / 8));

				const std::complex<real> zSin = std::sin(z);

				image(x, y) = hsvToRgb(Color(std::arg(zSin), 1, std::abs(zSin)));
			}
		}

		savePcx(image, "complex_sin.pcx");
	}

	void addTest()
	{
		testRunner().add("Complex.Sin", testSin);
	}

	CallFunction run(addTest);

}
