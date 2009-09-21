#ifndef PASTEL_DRAW_HORIZONTAL_LINE_HPP
#define PASTEL_DRAW_HORIZONTAL_LINE_HPP

#include "pastel/gfx/draw_horizontal_line.h"
#include "pastel/gfx/assigncolormixer.h"

#include "pastel/sys/syscommon.h"

namespace Pastel
{

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawHorizontalLine(
		real x, integer y, real x2,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		// Cull by y.

		if (y < 0 || y >= image.height())
		{
			return;
		}

		// Convert to integer coordinates.

		integer xMin = toPixelSpanPoint(x);
		integer xMax = toPixelSpanPoint(x2);

		// Cull by x.

		if (xMin >= image.width() || xMax <= 0)
		{
			return;
		}

		// Clip.

		if (xMin < 0)
		{
			xMin = 0;
		}

		if (xMax > image.width())
		{
			xMax = image.width();
		}

		typedef typename Image_View::Cursor Cursor;

		Cursor cursor = image.cursor(xMin, y);

		// Draw.

		for (integer x = xMin;x < xMax;++x)
		{
			*cursor = colorMixer(*cursor, color);
			cursor.xIncrement();
		}
	}

	template <typename Type, typename Image_View>
	void drawHorizontalLine(
		real x, integer y, real x2,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image)
	{
		drawHorizontalLine(
			x, y, x2, color, image, assignColorMixer<Type>());
	}

}

#endif
