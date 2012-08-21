// Description: Drawing a triangle
// Documentation: drawing.txt

#ifndef PASTEL_DRAW_TRIANGLE_H
#define PASTEL_DRAW_TRIANGLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"

#include "pastel/geometry/triangle.h"

namespace Pastel
{

	//! Draws a triangle with a solid color.

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawTriangle(
		const Triangle2& triangle,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer);

	//! Draws a triangle with a solid color.
	/*!
	This simply calls
	drawTriangle(triangle, color,
	image, assignColorMixer<Type>());
	*/

	template <typename Type, typename Image_View>
	void drawTriangle(
		const Triangle2& triangle,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image);

}

#include "pastel/gfx/draw_triangle.hpp"

#endif
