#include "pastel/gfx/labeltoimage.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/primes.h"

namespace Pastel
{

	PASTELGFX void labelToImage(
		const Array<2, int32>& labelImage,
		Array<2, uint8>& image)
	{
		const integer width = labelImage.width();
		const integer height = labelImage.height();

		Array<2, uint8> newImage(width, height);

		const int32 prime = followingPrime(300);

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				int32 data = labelImage(x, y);
				if (data > 255)
				{
					//data *= prime;
					data &= 0xFF;
				}

				newImage(x, y) = (uint8)data;
			}
		}

		image.swap(newImage);
	}

}
