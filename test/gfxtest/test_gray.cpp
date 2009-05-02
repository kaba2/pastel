#include "pastelgfxtest.h"

#include "pastel/sys/view_tools.h"
#include "pastel/sys/arrayview.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/loadpcx.h"
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
		Array<2, Color> image;
		loadPcx("lena.pcx", image);

		Array<2, Color> transformed(image.extent());

		transform(constArrayView(image),
			arrayView(transformed), lumaColor);

		savePcx(transformed, "output/gray_luma.pcx");

		transform(constArrayView(image),
			arrayView(transformed), lightness);

		savePcx(transformed, "output/gray_lightness.pcx");
	}

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

		savePcx(image, "output/gray_linear_luminance.pcx");
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

		savePcx(image, "output/gray_linear_luma.pcx");
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

		savePcx(image, "output/gray_linear_lightness.pcx");
	}

	void testAdd()
	{
		gfxTestList().add("Gray.Image", testGray);
		gfxTestList().add("Gray.Scale.Luma", testLinearLuma);
		gfxTestList().add("Gray.Scale.Lightness", testLinearLightness);
		gfxTestList().add("Gray.Scale.Luminance", testLinearLuminance);
	}

	CallFunction run(testAdd);

}
