#ifndef PASTELGFX_LABELING_TO_IMAGE_HPP
#define PASTELGFX_LABELING_TO_IMAGE_HPP

#include "pastel/gfx/labeling_to_image.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/hashing.h"

namespace Pastel
{

	inline void labelToImage(
		const Array<int32, 2>& labelImage,
		Array<uint8, 2>& image)
	{
		integer width = labelImage.width();
		integer height = labelImage.height();

		Array<uint8> newImage(Vector2i(width, height));

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				int32 data = labelImage(x, y);
				if (data > 255)
				{
					data = computeHash(data);
				}

				newImage(x, y) = (uint8)data;
			}
		}

		image.swap(newImage);
	}

}

#endif
