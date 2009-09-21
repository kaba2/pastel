#ifndef PASTEL_DRAW_FLOODFILL_HPP
#define PASTEL_DRAW_FLOODFILL_HPP

#include "pastel/gfx/draw_floodfill.h"

#include <vector>

namespace Pastel
{

	template <typename Type, typename Image_View>
	void floodFill(
		integer xStart, integer yStart,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image)
	{
		const integer width = image.width();
		const integer height = image.height();

		if (xStart < 0 || xStart >= width ||
			yStart < 0 || yStart >= height)
		{
			// Seed point out of range.
			return;
		}

		typedef typename Image_View::Cursor Cursor;

		const Type source = *image.cursor(xStart, yStart);

		if (source == color)
		{
			// No need to do anything.
			return;
		}

		std::vector<Integer2> stack;
		stack.push_back(Integer2(xStart, yStart));

		while (!stack.empty())
		{
			const Integer2 line = stack.back();
			stack.pop_back();

			// Find the extents of this scanline
			// and draw it.

			integer xLeft = line[0];
			integer xRight = line[0];
			const integer y = line[1];

			Cursor yCursor = image.cursor(xLeft, y);

			if (*yCursor != source)
			{
				continue;
			}

			{
				Cursor leftCursor = yCursor;
				leftCursor.xDecrement();

				while(xLeft > 0 && *leftCursor == source)
				{
					*leftCursor = color;

					--xLeft;
					leftCursor.xDecrement();
				}
			}

			{
				Cursor xyCursor = yCursor;

				while(xRight < width && *xyCursor == source)
				{
					*xyCursor = color;

					++xRight;
					xyCursor.xIncrement();
				}
			}

			// Recurse up and down.

			if (y < height - 1)
			{
				// Recurse top line.

				bool lineHandled = false;

				const integer yTop = y + 1;

				Cursor xyCursor = image.cursor(xLeft, yTop);

				for (integer x = xLeft;x < xRight;++x)
				{
					if (*xyCursor == source)
					{
						if (!lineHandled)
						{
							stack.push_back(Integer2(x, yTop));
							lineHandled = true;
						}
					}
					else
					{
						lineHandled = false;
					}

					xyCursor.xIncrement();
				}
			}
			if (y > 0)
			{
				// Recurse bottom line.

				bool lineHandled = false;

				const integer yBottom = y - 1;

				Cursor xyCursor = image.cursor(xLeft, yBottom);

				for (integer x = xLeft;x < xRight;++x)
				{
					if (*xyCursor == source)
					{
						if (!lineHandled)
						{
							stack.push_back(Integer2(x, yBottom));
							lineHandled = true;
						}
					}
					else
					{
						lineHandled = false;
					}

					xyCursor.xIncrement();
				}
			}
		}
	}

}

#endif
