// Description: Drawing a horizontal line segment
// Documentation: drawing.txt

#ifndef PASTELGFX_DRAW_HORIZONTAL_LINE_H
#define PASTELGFX_DRAW_HORIZONTAL_LINE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view/view.h"

namespace Pastel
{

	//! Draws a horizontal line segment.

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawHorizontalLine(
		dreal x, integer y, dreal x2,
		const NoDeduction<Type>& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer);

	//! Draws a horizontal line segment.

	template <typename Type, typename Image_View>
	void drawHorizontalLine(
		dreal x, integer y, dreal x2,
		const NoDeduction<Type>& color,
		const View<2, Type, Image_View>& image);

}

#include "pastel/gfx/drawing/draw_horizontal_line.hpp"

#endif
