#include "pastel/gfx/image_tools.h"

#include "pastel/gfx/color_tools.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	PASTELGFX void indexedImageToColorImage(
		const Array<uint8, 2>& indexedImage,
		const std::vector<Color>& palette,
		Array<Color, 2>& result)
	{
		ENSURE_OP(palette.size(), ==, 256);

		const integer width = indexedImage.width();
		const integer height = indexedImage.height();

		Array<Color, 2> colorImage(Vector2i(width, height));
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
