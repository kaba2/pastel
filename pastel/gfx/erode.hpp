#ifndef PASTEL_ERODE_HPP
#define PASTEL_ERODE_HPP

#include "pastel/gfx/erode.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	namespace Detail
	{

		namespace Erode
		{

			template <
				typename Image_ConstView,
				typename Element_ConstView>
			bool contains(
				const ExtendedConstView<2, bool, Image_ConstView>& image,
				const ConstView<2, bool, Element_ConstView>& element,
				integer x,
				integer y)
			{
				const integer elementWidth = element.width();
				const integer elementHeight = element.height();

				const integer width = image.width();
				const integer height = image.height();

				for (integer i = 0;i < elementHeight;++i)
				{
					for (integer j = 0;j < elementWidth;++j)
					{
						if (element(j, i) &&
							!image(x + j, y + i))
						{
							return false;
						}
					}
				}

				return true;
			}

		}

	}

	template <
		typename Input_ConstView,
		typename Element_ConstView,
		typename Output_View>
		void erode(
		const ExtendedConstView<2, bool, Input_ConstView>& input,
		const ConstView<2, bool, Element_ConstView>& element,
		const View<2, bool, Output_View>& output,
		integer xPivot,
		integer yPivot)
	{
		const integer width = input.width();
		const integer height = input.height();

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				output(x, y) = Detail::Erode::contains(input, element, x - xPivot, y - yPivot);
			}
		}
	}

	template <
		typename Input_ConstView,
		typename Element_ConstView,
		typename Output_View>
		void erode(
		const ExtendedConstView<2, bool, Input_ConstView>& input,
		const ConstView<2, bool, Element_ConstView>& element,
		const View<2, bool, Output_View>& output)
	{
		const Point<integer, 2> center(
			element.extent() / 2);
		erode(input, element, output, center.x(), center.y());
	}

}

#endif
