// Description: Testing for HSV color space
// DocumentationOf: color_hsv.h

#include "pastelgfxtest.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/color_space.h"

using namespace Pastel;

namespace
{

	void testHsv()
	{
		const real Width = 512;
		const real Height = 128;
		Array<Color, 2> image(Width, Height);

		const real32 HueStep = (real32)1 / Width;
		const real32 SaturationStep = (real32)1 / Height;

		for (integer x = 0;x < Width;++x)
		{
			for (integer y = 0;y < Height;++y)
			{
				image(x, y) = hsvToRgb(
					Color(HueStep * x, SaturationStep * y, 1));
			}
		}

		savePcx(image, "output/hsv_scale.pcx");
	}

	void test()
	{
		testHsv();
	}

	void addTest()
	{
		gfxTestList().add("Hsv", test);
	}

	CallFunction run(addTest);

}
