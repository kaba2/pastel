#ifndef PASTEL_RECONSTRUCTIMAGE_HPP
#define PASTEL_RECONSTRUCTIMAGE_HPP

#include "pastel/dsp/reconstructimage.h"

namespace Pastel
{

	template <typename ImageType,
		typename FilterType,
		typename ResultType>
		void reconstructImage(
		const Array<ImageType, 2>& image,
		const Array<bool, 2>& reconstructArea,
		const Array<FilterType, 2>& filter,
		Array<ResultType, 2>& result)
	{
		ENSURE(addressOf(image) != addressOf(result));
		ENSURE(addressOf(filter) != addressOf(result));
		ENSURE(odd(filter.width()));
		ENSURE(odd(filter.height()));

		result.setExtent(image.width(), image.height());

		const integer imageWidth = image.width();
		const integer imageHeight = image.height();

		Array<FilterType, 2> weightImage(
			imageWidth, imageHeight, 0);

		result = image;

		const integer filterWidth = filter.width() / 2;
		const integer filterHeight = filter.height() / 2;

		// Reconstruct the spectrum image
		// at pixel locations.

		for (integer y = 0;y < imageHeight;++y)
		{
			for (integer x = 0;x < imageWidth;++x)
			{
				if (reconstructArea(x, y))
				{
					integer uOffset = 0;
					integer vOffset = 0;

					integer xBegin = x - filterWidth;
					if (xBegin < 0)
					{
						uOffset = -xBegin;
						xBegin = 0;
					}

					integer yBegin = y - filterHeight;
					if (yBegin < 0)
					{
						vOffset = -yBegin;
						yBegin = 0;
					}

					const integer xEnd = std::min(
						x + filterWidth, imageWidth - 1);
					const integer yEnd = std::min(
						y + filterHeight, imageHeight - 1);

					const integer areaWidth = (xEnd - xBegin) + 1;
					const integer areaHeight = (yEnd - yBegin) + 1;

					result(x, y) = 0;

					for (integer v = 0;v < areaHeight;++v)
					{
						for (integer u = 0;u < areaWidth;++u)
						{
							if (!reconstructArea(
								xBegin + u, yBegin + v))
							{
								const FilterType weight(
									filter(uOffset + u, vOffset + v));

								result(x, y) += image(
									xBegin + u, yBegin + v) * weight;
								weightImage(x, y) += weight;
							}
						}
					}
				}
			}
		}

		// Normalize the spectrum image.

		for (integer y = 0;y < imageHeight;++y)
		{
			for (integer x = 0;x < imageWidth;++x)
			{
				const FilterType weight(weightImage(x, y));
				if (weight != 0)
				{
					result(x, y) /= weight;
				}
			}
		}
	}

}

#endif
