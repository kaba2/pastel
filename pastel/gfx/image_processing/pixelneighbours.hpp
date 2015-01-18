#ifndef PASTELGFX_PIXELNEIGHBOURS_HPP
#define PASTELGFX_PIXELNEIGHBOURS_HPP

#include "pastel/gfx/image_processing/pixelneighbours.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	namespace
	{

		inline bool check(const Array<bool, 2>& image,
			integer x, integer y)
		{
			if (x < 0 || x >= image.width() ||
				y < 0 || y >= image.height())
			{
				return false;
			}

			return image(x, y);
		}

	}

	inline integer pixelNeighbours(
		const Array<bool, 2>& image,
		integer x, integer y)
	{
		integer width = image.width();
		integer height = image.height();


		ENSURE2(x >= 0 && x < width, x, width);
		ENSURE2(y >= 0 && y < height, y, height);

		integer result = 0;

		if (check(image, x + 1, y))
		{
			++result;
		}
		if (check(image, x + 1, y + 1))
		{
			++result;
		}
		if (check(image, x, y + 1))
		{
			++result;
		}
		if (check(image, x - 1, y + 1))
		{
			++result;
		}
		if (check(image, x - 1, y))
		{
			++result;
		}
		if (check(image, x - 1, y - 1))
		{
			++result;
		}
		if (check(image, x, y - 1))
		{
			++result;
		}
		if (check(image, x + 1, y - 1))
		{
			++result;
		}

		return result;
	}

	inline uint32 pixelNeighbourMask(
		const Array<bool, 2>& image,
		integer x, integer y)
	{
		integer width = image.width();
		integer height = image.height();


		ENSURE2(x >= 0 && x < width, x, width);
		ENSURE2(y >= 0 && y < height, y, height);

		uint32 result = 0;

		if (check(image, x + 1, y))
		{
			result += 1;
		}
		if (check(image, x + 1, y + 1))
		{
			result += 2;
		}
		if (check(image, x, y + 1))
		{
			result += 4;
		}
		if (check(image, x - 1, y + 1))
		{
			result += 8;
		}
		if (check(image, x - 1, y))
		{
			result += 16;
		}
		if (check(image, x - 1, y - 1))
		{
			result += 32;
		}
		if (check(image, x, y - 1))
		{
			result += 64;
		}
		if (check(image, x + 1, y - 1))
		{
			result += 128;
		}

		return result;
	}

}

#endif
