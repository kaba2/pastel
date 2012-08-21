// Description: Drawing a pixel
// Documentation: drawing.txt

#ifndef PASTEL_DRAW_PIXEL_H
#define PASTEL_DRAW_PIXEL_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Draws a pixel.

	template <int N, typename Type, typename Image_View, typename ColorMixer>
	void drawPixel(
		const PASTEL_NO_DEDUCTION((Vector<integer, N>))& position,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<N, Type, Image_View>& image,
		const ColorMixer& colorMixer);

	//! Draws a pixel.

	template <int N, typename Type, typename Image_View>
	void drawPixel(
		const PASTEL_NO_DEDUCTION((Vector<integer, N>))& position,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<N, Type, Image_View>& image);

}

#include "pastel/gfx/draw_pixel.hpp"

#endif
