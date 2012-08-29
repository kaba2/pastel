#ifndef PASTELGFX_DRAW_VERTICAL_LINE_HPP
#define PASTELGFX_DRAW_VERTICAL_LINE_HPP

#include "pastel/gfx/draw_vertical_line.h"
#include "pastel/gfx/assign_colormixer.h"

#include "pastel/sys/syscommon.h"

namespace Pastel
{

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawVerticalLine(
		real x, integer y, real y2,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		// Cull by x.

		if (x < 0 || x >= image.width())
		{
			return;
		}

		// Convert to integer coordinates.

		integer yMin = toPixelSpanPoint(y);
		integer yMax = toPixelSpanPoint(y2);

		// Cull by x.

		if (yMin >= image.height() || yMax <= 0)
		{
			return;
		}

		// Clip.

		if (yMin < 0)
		{
			yMin = 0;
		}

		if (yMax > image.height())
		{
			yMax = image.height();
		}

		typedef typename Image_View::Cursor Cursor;

		Cursor cursor = image.cursor(x, yMin);

		// Draw.

		for (integer y = yMin;y < yMax;++y)
		{
			*cursor = colorMixer(*cursor, color);
			cursor.yIncrement();
		}
	}

	template <typename Type, typename Image_View>
	void drawVerticalLine(
		real x, integer y, real y2,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image)
	{
		drawVerticalLine(
			x, y, y2, color, image, 
			assignColorMixer<Type>());
	}

}

#endif
