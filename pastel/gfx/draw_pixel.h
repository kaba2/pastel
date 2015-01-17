// Description: Drawing a pixel
// Documentation: drawing.txt

#ifndef PASTELGFX_DRAW_PIXEL_H
#define PASTELGFX_DRAW_PIXEL_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view/view.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Draws a pixel.

	template <int N, typename Type, typename Image_View, typename ColorMixer>
	void drawPixel(
		const NoDeduction<Vector<integer, N>>& position,
		const NoDeduction<Type>& color,
		const View<N, Type, Image_View>& image,
		const ColorMixer& colorMixer);

	//! Draws a pixel.

	template <int N, typename Type, typename Image_View>
	void drawPixel(
		const NoDeduction<Vector<integer, N>>& position,
		const NoDeduction<Type>& color,
		const View<N, Type, Image_View>& image);

}

#include "pastel/gfx/draw_pixel.hpp"

#endif
