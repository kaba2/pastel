#include "pastel/gfx/box_labeling.h"
#include "pastel/gfx/drawing.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	PASTELGFX integer labelWithBoxes(
		const Array<bool, 2>& image,
		integer maxWidth,
		integer maxHeight,
		real maxRatio,
		Array<int32, 2>& labelImage)
	{
		// This is a greedy algorithm to segment a binary image
		// into boxes.
		// The algorithm is to
		// 1) Traverse pixels left to right, bottom to top.
		// 2) If you find a non-labeled pixel, find a maximal non-labeled square box
		//    that extends to the right and up.
		// 3) If square boxes are not required, starting from the square box
		//    of the previous step, find a maximal non-labeled box
		//    that extends to the right and up.
		//
		// Note that the 'maximal' includes the possible limitations
		// from 'maxWidth' and 'maxHeight'.

		ENSURE_OP(maxWidth, >, 0);
		ENSURE_OP(maxHeight, >, 0);
		ENSURE_OP(image.width(), <=, labelImage.width());
		ENSURE_OP(image.height(), <=, labelImage.height());

		if (maxWidth > image.width())
		{
			maxWidth = image.width();
		}
		if (maxHeight > image.height())
		{
			maxHeight = image.height();
		}

		const integer minMaxDimension = std::min(maxWidth, maxHeight) >> 1;

		integer minMinDimension = minMaxDimension;

		const integer width = image.width();
		const integer height = image.height();

		drawBox(AlignedBox2(0, 0, width, height), 0, arrayView(labelImage));

		integer labelCounter = 1;
		integer roundCounter = 1;

		bool stillWorkTodo = true;
		integer maxAcceptPointedDimension = 0;

		while(stillWorkTodo)
		{
			log() << "Round " << roundCounter
				<< ": min dimension " << minMinDimension << logNewLine;

			stillWorkTodo = false;
			maxAcceptPointedDimension = 0;

			for (integer y = 0;y < height;++y)
			{
				for (integer x = 0;x < width;++x)
				{
					if (image(x, y) && labelImage(x, y) <= 0 && labelImage(x, y) > -roundCounter)
					{
						integer boxSize = 1;
						while(boxSize < minMaxDimension &&
							x + boxSize < width && y + boxSize < height)
						{
							bool boxFits = true;
							{
								const integer v = y + boxSize;
								for (integer u = x;u < x + boxSize + 1;++u)
								{
									if (!image(u, v) || labelImage(u, v) > 0)
									{
										boxFits = false;
										break;
									}
								}
							}
							if (boxFits)
							{
								const integer u = x + boxSize;
								// No need to check v = y + boxSize, already done.
								for (integer v = y;v < y + boxSize;++v)
								{
									if (!image(u, v) || labelImage(u, v) > 0)
									{
										boxFits = false;
										break;
									}
								}
							}
							if (!boxFits)
							{
								break;
							}
							++boxSize;
						}

						integer boxWidth = boxSize;
						integer boxHeight = boxSize;

						if (maxRatio > 1)
						{
							// Try to extend in the y direction
							while(boxHeight < maxHeight &&
								y + boxHeight < height &&
								std::max((real)boxWidth / (boxHeight + 1),
								(real)(boxHeight + 1) / boxWidth) <= maxRatio)
							{
								bool boxFits = true;

								const integer v = y + boxHeight;
								for (integer u = x;u < x + boxWidth;++u)
								{
									if (!image(u, v) || labelImage(u, v) > 0)
									{
										boxFits = false;
										break;
									}
								}

								if (!boxFits)
								{
									break;
								}
								++boxHeight;
							}

							// Try to extend in the x direction
							while(boxWidth < maxWidth &&
								x + boxWidth < width &&
								std::max((real)(boxWidth + 1) / boxHeight,
								(real)boxHeight / (boxWidth + 1)) <= maxRatio)
							{
								bool boxFits = true;

								const integer u = x + boxWidth;
								for (integer v = y;v < y + boxHeight;++v)
								{
									if (!image(u, v) || labelImage(u, v) > 0)
									{
										boxFits = false;
										break;
									}
								}

								if (!boxFits)
								{
									break;
								}
								++boxWidth;
							}
						}

						const integer minDimension =
							std::min(boxWidth, boxHeight);

						if (minDimension >= minMinDimension)
						{
							if (minDimension > maxAcceptPointedDimension)
							{
								maxAcceptPointedDimension = minDimension;
							}
							drawBox(AlignedBox2(x, y, x + boxWidth, y + boxHeight), labelCounter,
								arrayView(labelImage));
							++labelCounter;
						}
						else
						{
							// Mark as todo
							drawBox(AlignedBox2(x, y, x + boxWidth, y + boxHeight), -roundCounter,
								arrayView(labelImage));
							stillWorkTodo = true;
						}
					}
				}
			}

			if (maxAcceptPointedDimension > 0)
			{
				if ((maxAcceptPointedDimension >> 1) < (minMinDimension >> 1))
				{
					minMinDimension = maxAcceptPointedDimension >> 1;
				}
				else
				{
					minMinDimension >>= 1;
				}
			}
			else
			{
				minMinDimension >>= 1;
			}
			++roundCounter;
		}

		log() << "Labeling took " << roundCounter - 1 << " rounds." << logNewLine;

		return labelCounter;
	}

}
