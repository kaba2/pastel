#include "pastelgfxtest.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/color_tools.h"

using namespace Pastel;

namespace
{

	void testLinearLuminance()
	{
		const real Width = 400;
		const real Height = 100;
		Array<2, Color> image(Width, Height);

		const real32 Step = 1 / Width;

		for (integer x = 0;x < Width;++x)
		{
			const Color color(
				linearSrgbToGammaSrgb(
				Color(Step * x)));
			for (integer y = 0;y < Height;++y)
			{
				image(x, y) = color;
			}
		}

		savePcx(image, "testgamma_linear_luminance.pcx");
	}

	void testLinearLuma()
	{
		const real Width = 400;
		const real Height = 100;
		Array<2, Color> image(Width, Height);

		const real32 Step = 1 / Width;

		for (integer x = 0;x < Width;++x)
		{
			const Color color(
				Color(Step * x));
			for (integer y = 0;y < Height;++y)
			{
				image(x, y) = color;
			}
		}

		savePcx(image, "testgamma_linear_luma.pcx");
	}

	void testLinearLightness()
	{
		const real Width = 400;
		const real Height = 100;
		Array<2, Color> image(Width, Height);

		const real32 Step = 100 / Width;

		for (integer x = 0;x < Width;++x)
		{
			const Color color(
				xyzToSrgb(labToXyz(Color(Step * x, 0, 0))));
			for (integer y = 0;y < Height;++y)
			{
				image(x, y) = color;
			}
		}

		savePcx(image, "testgamma_linear_lightness.pcx");
	}

	void testBegin()
	{
		testLinearLuma();
		testLinearLuminance();
		testLinearLightness();
	}

	void testAdd()
	{
		gfxTestList().add("Gamma", testBegin);
	}

	CallFunction run(testAdd);

}
