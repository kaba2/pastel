#ifndef PASTEL_DRAW_BOX_HPP
#define PASTEL_DRAW_BOX_HPP

#include "pastel/gfx/draw_box.h"
#include "pastel/gfx/assigncolormixer.h"

#include "pastel/sys/rectangle.h"

namespace Pastel
{

	template <typename Type, typename Image_View>
	void drawBox(
		const AlignedBox2& box,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image)
	{
		Pastel::drawBox(box, color, image, assignColorMixer<Type>());
	}

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawBox(
		const AlignedBox2& box,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		const Rectangle2 discreteBox(
			toPixelSpanPoint(box.min()), 
			toPixelSpanPoint(box.max()));

		Rectangle2 clippedBox;
		if (!intersect(discreteBox, 
			Rectangle2(Vector2i(0), image.extent()), 
			clippedBox))
		{
			return;
		}

		typedef typename Image_View::Cursor Cursor;

		Cursor yCursor = image.cursor(clippedBox.min());

		const integer width = clippedBox.extent().x();
		const integer height = clippedBox.extent().y();

		for (integer y = 0;y < height;++y)
		{
			Cursor xyCursor = yCursor;

			for (integer x = 0;x < width;++x)
			{
				*xyCursor = colorMixer(*xyCursor, color);
				xyCursor.xIncrement();
			}

			yCursor.yIncrement();
		}
	}

}

#endif
