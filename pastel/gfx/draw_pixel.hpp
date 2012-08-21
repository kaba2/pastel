#ifndef PASTEL_DRAW_PIXEL_HPP
#define PASTEL_DRAW_PIXEL_HPP

#include "pastel/gfx/draw_pixel.h"
#include "pastel/gfx/assign_colormixer.h"

#include "pastel/sys/vector_compare.h"

namespace Pastel
{

	template <int N, typename Type, typename Image_View, typename ColorMixer>
	void drawPixel(
		const PASTEL_NO_DEDUCTION((Vector<integer, N>))& position,
		const PASTEL_NO_DEDUCTION(Type)& color,
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

	template <int N, typename Type, typename Image_View>
	void drawPixel(
		const PASTEL_NO_DEDUCTION((Vector<integer, N>))& position,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<N, Type, Image_View>& image)
	{
		drawPixel(position, color, image, AssignColorMixer<Type>());
	}

}

#endif
