// Description: Testing for HSV color space
// DocumentationOf: color_hsv.h

#include "test/test_init.h"

#include "pastel/gfx/image_file.h"
#include "pastel/gfx/color.h"

TEST_CASE("Hsv (Hsv)")
{
	real Width = 512;
	real Height = 128;
	Array<Color, 2> image(Vector2i(Width, Height));

	real32 HueStep = (real32)1 / Width;
	real32 SaturationStep = (real32)1 / Height;

	for (integer x = 0;x < Width;++x)
	{
		for (integer y = 0;y < Height;++y)
		{
			image(x, y) = hsvToRgb(
				Color(HueStep * x, SaturationStep * y, 1));
		}
	}

	savePcx(image, "hsv_scale.pcx");
}

