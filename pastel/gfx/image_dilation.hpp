#ifndef PASTELGFX_DILATE_HPP
#define PASTELGFX_DILATE_HPP

#include "pastel/gfx/image_dilation.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/addressof.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/intersect_alignedbox_alignedbox.h"

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
		integer elementWidth = element.width();
		integer elementHeight = element.height();

		integer width = image.width();
		const integer height = image.height();

		const AlignedBox2i elementWindow(
			x, y, x + elementWidth, y + elementHeight);
		const AlignedBox2i imageWindow(
			0, 0, width, height);

		AlignedBox2i clippedWindow;

		if (intersect(elementWindow, imageWindow, clippedWindow))
		{
			clippedWindow.min() -= Vector2i(x, y);
			clippedWindow.max() -= Vector2i(x, y);

			const integer clippedWidth = clippedWindow.width();
			const integer clippedHeight = clippedWindow.height();

			const integer xMinClipped = clippedWindow.min().x();
			const integer yMinClipped = clippedWindow.min().y();

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
		integer width = input.width();
		integer height = input.height();

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
