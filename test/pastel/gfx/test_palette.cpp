// Description: Testing for palettes
// DocumentationOf: color_palette.h

#include "test/test_init.h"

#include "pastel/sys/view.h"

#include "pastel/gfx/image_file.h"
#include "pastel/gfx/color.h"
#include "pastel/gfx/drawing.h"

#include <iostream>
#include <string>

namespace
{

	void savePalette(
		const std::vector<Color>& palette,
		const std::string& fileName)
	{
		integer colorWidth = 32;
		integer colorHeight = 32;

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

}

TEST_CASE("Palette (Palette)")
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

