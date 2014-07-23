#ifndef PASTELGFX_IMAGE_ERODING_HPP
#define PASTELGFX_IMAGE_ERODING_HPP

#include "pastel/gfx/image_eroding.h"

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
				integer elementWidth = element.width();
				const integer elementHeight = element.height();

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
		integer width = input.width();
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
		Vector<integer, 2> center(
			element.extent() / 2);
		erode(input, element, output, center.x(), center.y());
	}

}

#endif
