// Description: Drawing a vertical line segment
// Documentation: drawing.txt

#ifndef PASTELGFX_DRAW_VERTICAL_LINE_H
#define PASTELGFX_DRAW_VERTICAL_LINE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"

namespace Pastel
{

	//! Draws a vertical line segment.

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawVerticalLine(
		real x, integer y, real y2,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer);

	//! Draws a horizontal line segment.

	template <typename Type, typename Image_View>
	void drawVerticalLine(
		real x, integer y, real y2,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image);

}

#include "pastel/gfx/draw_vertical_line.hpp"

#endif
