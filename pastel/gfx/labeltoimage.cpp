#include "pastel/gfx/labeltoimage.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/primes.h"

namespace Pastel
{

	PASTELGFX void labelToImage(
		const Array<int32, 2>& labelImage,
		Array<uint8, 2>& image)
	{
		const integer width = labelImage.width();
		const integer height = labelImage.height();

		Array<uint8, 2> newImage(width, height);

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
