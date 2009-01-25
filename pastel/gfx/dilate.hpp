#ifndef PASTELGFX_DILATE_HPP
#define PASTELGFX_DILATE_HPP

#include "pastel/gfx/dilate.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/addressof.h"
#include "pastel/syS/view_tools.h"

namespace Pastel
{

	template <
		typename Element_ConstView,
		typename Image_View>
		void logicOr(
		const ConstView<2, bool, Element_ConstView>& element,
		integer x,
		integer y,
		const View<2, bool, Image_View>& image)
	{
		const integer elementWidth = element.width();
		const integer elementHeight = element.height();

		const integer width = image.width();
		const integer height = image.height();

		const Rectangle2 elementWindow(
			x, y, x + elementWidth, y + elementHeight);
		const Rectangle2 imageWindow(
			0, 0, width, height);

		Rectangle2 clippedWindow;

		if (intersect(elementWindow, imageWindow, clippedWindow))
		{
			clippedWindow.min() -= IVector2(x, y);
			clippedWindow.max() -= IVector2(x, y);

			const integer clippedWidth = clippedWindow.width();
			const integer clippedHeight = clippedWindow.height();

			const integer xMinClipped = clippedWindow.xMin();
			const integer yMinClipped = clippedWindow.yMin();

			for (integer i = 0;i < clippedHeight;++i)
			{
				for (integer j = 0;j < clippedWidth;++j)
				{
					if (element(xMinClipped + j, yMinClipped + i))
					{
						image(x + j, y + i) = true;
					}
				}
			}
		}
	}

	template <
		typename Input_ConstView,
		typename Element_ConstView,
		typename Output_View>
		void dilate(
		const ConstView<2, bool, Input_ConstView>& input,
		const ConstView<2, bool, Element_ConstView>& element,
		integer xPivot,
		integer yPivot,
		const View<2, bool, Output_View>& output)
	{
		const integer width = input.width();
		const integer height = input.height();

		clear(false, output);

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				if (input(x, y))
				{
					logicOr(element, x - xPivot, y - yPivot, output);
				}
			}
		}
	}

}

#endif
