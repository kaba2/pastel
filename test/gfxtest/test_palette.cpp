// Description: Testing for palettes
// DocumentationOf: color_palette.h

#include "test_pastelgfx.h"

#include "pastel/sys/arrayview.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/color_palette.h"

#include <iostream>
#include <string>

using namespace std;
using namespace Pastel;

namespace
{

	void savePalette(
		const std::vector<Color>& palette,
		const std::string& fileName)
	{
		const integer colorWidth = 32;
		const integer colorHeight = 32;

		const integer width = colorWidth * 16;
		const integer height = colorHeight * 16;

		Array<Color, 2> image(Vector2i(width, height));

		for (integer y = 0;y < 16;++y)
		{
			for (integer x = 0;x < 16;++x)
			{
				drawBox(AlignedBox2(x * colorWidth, y * colorHeight,
					(x + 1) * colorWidth, (y + 1) * colorHeight),
					palette[y * 16 + x],
					arrayView(image));
			}
		}

		savePcx(image, fileName);
	}

	void testPalette()
	{
		std::vector<Color> palette;

		grayscalePalette(palette, 256);

		savePalette(palette, "palette_grayscale.pcx");

		vgaPalette(palette);

		savePalette(palette, "palette_vga.pcx");

		smoothFalseColorPalette(palette, 256);

		savePalette(palette, "palette_smoothfalsecolor.pcx");

		falseColorPalette(palette, 256);

		savePalette(palette, "palette_falsecolor.pcx");

		randomPalette(palette, 256);

		savePalette(palette, "palette_random.pcx");
}

	void test()
	{
		testPalette();
	}

	void addTest()
	{
		testRunner().add("Palette", test);
	}

	CallFunction run(addTest);

}
