// Description: Drawing a horizontal line segment

#ifndef PASTEL_DRAW_HORIZONTAL_LINE_H
#define PASTEL_DRAW_HORIZONTAL_LINE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"

namespace Pastel
{

	//! Draws a horizontal line segment.

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawHorizontalLine(
		real x, integer y, real x2,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer);

	//! Draws a horizontal line segment.

	template <typename Type, typename Image_View>
	void drawHorizontalLine(
		real x, integer y, real x2,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image);

}

#include "pastel/gfx/draw_horizontal_line.hpp"

#endif
