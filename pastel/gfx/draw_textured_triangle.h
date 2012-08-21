// Description: Drawing a textured triangle
// Documentation: drawing.txt

#ifndef PASTEL_DRAW_TEXTURED_TRIANGLE_H
#define PASTEL_DRAW_TEXTURED_TRIANGLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"

#include "pastel/gfx/texture.h"

#include "pastel/geometry/triangle.h"

namespace Pastel
{

	//! Draws a textured triangle.

	template <typename Type,
		typename Image_View, typename ColorMixer>
	void drawTriangle(
		const Triangle2& triangle,
		const Triangle2& uvTriangle,
		const Texture<Type>& textureSampler,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer);

	//! Draws a textured triangle.
	/*!
	This simply calls
	drawTriangle(triangle, uvTriangle, textureSampler,
	image, assignColorMixer<Type>());
	*/

	template <typename Type,
		typename Image_View>
	void drawTriangle(
		const Triangle2& triangle,
		const Triangle2& uvTriangle,
		const Texture<Type>& textureSampler,
		const View<2, Type, Image_View>& image);

}

#include "pastel/gfx/draw_textured_triangle.hpp"

#endif
