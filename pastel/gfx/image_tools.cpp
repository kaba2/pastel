#include "pastel/gfx/image_tools.h"

#include "pastel/gfx/color_tools.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	PASTELGFX void indexedImageToColorImage(
		const LinearArray<2, uint8>& indexedImage,
		const std::vector<Color>& palette,
		LinearArray<2, Color>& result)
	{
		ENSURE1(palette.size() == 256, palette.size());

		const integer width = indexedImage.width();
		const integer height = indexedImage.height();

		LinearArray<2, Color> colorImage(width, height);
		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				colorImage(x, y) = palette[indexedImage(x, y)];
			}
		}

		colorImage.swap(result);
	}

}
