// Description: Testing for lightness
// DocumentationOf: color_tools.h

#include "test_pastelgfx.h"

#include "pastel/sys/view_tools.h"
#include "pastel/sys/arrayview.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/color_space.h"
#include "pastel/gfx/color_tools.h"

using namespace Pastel;

namespace
{

	Color lumaColor(const Color& rgb)
	{
		return Color(luma(rgb));
	}

	Color lightness(const Color& rgb)
	{
		Color lab(xyzToLab(srgbToXyz(rgb)));
		lab[1] = 0;
		lab[2] = 0;
		return xyzToSrgb(labToXyz(lab));
	}

	void testGray()
	{
		Array<Color, 2> image;
		loadPcx("lena.pcx", image);

		Array<Color, 2> transformed(image.extent());

		transform(constArrayView(image),
			arrayView(transformed), lumaColor);

		savePcx(transformed, "gray_luma.pcx");

		transform(constArrayView(image),
			arrayView(transformed), lightness);

		savePcx(transformed, "gray_lightness.pcx");
	}

	void testLinearLuminance()
	{
		real Width = 400;
		const real Height = 100;
		Array<Color, 2> image(Vector2i(Width, Height));

		const real32 Step = 1 / Width;

		for (integer x = 0;x < Width;++x)
		{
			const Color color(
				linearSrgbToSrgb(

				Color(Step * x)));
			for (integer y = 0;y < Height;++y)
			{
				image(x, y) = color;
			}
		}

		savePcx(image, "gray_linear_luminance.pcx");
	}

	void testLinearLuma()
	{
		real Width = 400;
		const real Height = 100;
		Array<Color, 2> image(Vector2i(Width, Height));

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

		savePcx(image, "gray_linear_luma.pcx");
	}

	void testLinearLightness()
	{
		real Width = 400;
		const real Height = 100;
		Array<Color, 2> image(Vector2i(Width, Height));

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

		savePcx(image, "gray_linear_lightness.pcx");
	}

	void addTest()
	{
		testRunner().add("Gray.Image", testGray);
		testRunner().add("Gray.Scale.Luma", testLinearLuma);
		testRunner().add("Gray.Scale.Lightness", testLinearLightness);
		testRunner().add("Gray.Scale.Luminance", testLinearLuminance);
	}

	CallFunction run(addTest);

}
