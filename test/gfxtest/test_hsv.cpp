#include "pastelgfxtest.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/color_space.h"

using namespace Pastel;

namespace
{

	void testHsv()
	{
		const real Width = 400;
		const real Height = 100;
		Array<Color, 2> image(Width, Height);

		const real32 Step = (real32)1 / Width;

		for (integer x = 0;x < Width;++x)
		{
			const Color color(
				hsvToRgb(Color(Step * x, 1, 1)));
			for (integer y = 0;y < Height;++y)
			{
				image(x, y) = color;
			}
		}

		savePcx(image, "output/hsv_scale.pcx");
	}

	void testBegin()
	{
		testHsv();
	}

	void testAdd()
	{
		gfxTestList().add("Hsv", testBegin);
	}

	CallFunction run(testAdd);

}
