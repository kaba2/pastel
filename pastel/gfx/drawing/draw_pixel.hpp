#ifndef PASTELGFX_DRAW_PIXEL_HPP
#define PASTELGFX_DRAW_PIXEL_HPP

#include "pastel/gfx/drawing/draw_pixel.h"
#include "pastel/gfx/color/colormixer/assign_colormixer.h"

#include "pastel/sys/vector/vector_compare.h"

namespace Pastel
{

	template <integer N, typename Type, typename Image_View, typename ColorMixer>
	void drawPixel(
		const NoDeduction<Vector<integer, N>>& position,
		const NoDeduction<Type>& color,
		const View<N, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		if (anyLess(position, 0) ||
			anyGreaterEqual(position, image.extent()))
		{
			return;
		}

		image(position) = colorMixer(image(position), color);
	}

	template <integer N, typename Type, typename Image_View>
	void drawPixel(
		const NoDeduction<Vector<integer, N>>& position,
		const NoDeduction<Type>& color,
		const View<N, Type, Image_View>& image)
	{
		drawPixel(position, color, image, AssignColorMixer<Type>());
	}

}

#endif
