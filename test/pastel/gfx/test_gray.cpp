// Description: Testing for lightness
// DocumentationOf: color_tools.h

#include "test/test_init.h"

#include "pastel/sys/view.h"

#include "pastel/gfx/image_file.h"
#include "pastel/gfx/color.h"

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

}

TEST_CASE("Gray (Gray)")
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

TEST_CASE("LinearLuminance (LinearLuminance)")
{
	real Width = 400;
	real Height = 100;
	Array<Color, 2> image(Vector2i(Width, Height));

	real32 Step = 1 / Width;

	for (integer x = 0;x < Width;++x)
	{
		Color color(
			linearSrgbToSrgb(
			Color(Step * x)));
		for (integer y = 0;y < Height;++y)
		{
			image(x, y) = color;
		}
	}

	savePcx(image, "gray_linear_luminance.pcx");
}

TEST_CASE("LinearLuma (LinearLuma)")
{
	real Width = 400;
	real Height = 100;
	Array<Color, 2> image(Vector2i(Width, Height));

	real32 Step = 1 / Width;

	for (integer x = 0;x < Width;++x)
	{
		Color color(
			Color(Step * x));
		for (integer y = 0;y < Height;++y)
		{
			image(x, y) = color;
		}
	}

	savePcx(image, "gray_linear_luma.pcx");
}

TEST_CASE("LinearLightness (LinearLightness)")
{
	real Width = 400;
	real Height = 100;
	Array<Color, 2> image(Vector2i(Width, Height));

	real32 Step = 100 / Width;

	for (integer x = 0;x < Width;++x)
	{
		Color color(
			xyzToSrgb(labToXyz(Color(Step * x, 0, 0))));
		for (integer y = 0;y < Height;++y)
		{
			image(x, y) = color;
		}
	}

	savePcx(image, "gray_linear_lightness.pcx");
}
