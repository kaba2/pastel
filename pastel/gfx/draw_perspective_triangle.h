// Description: Drawing a textured triangle in perspective

#ifndef PASTEL_DRAW_PERSPECTIVE_TRIANGLE_H
#define PASTEL_DRAW_PERSPECTIVE_TRIANGLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"

#include "pastel/geometry/triangle.h"

#include "pastel/gfx/texture.h"

namespace Pastel
{

	//! Draws a triangle in perspective.

	template <typename Type,
		typename Image_View, typename ColorMixer>
	void drawTriangle(
		const Triangle3& triangle,
		const Triangle2& uvTriangle,
		const Texture<Type>& textureSampler,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer);

	//! Draws a triangle in perspective.
	/*!
	This simply calls
	drawTriangle(triangle, uvTriangle, textureSampler,
	image, assignColorMixer<Type>());
	*/

	template <typename Type,
		typename Image_View>
	void drawTriangle(
		const Triangle3& triangle,
		const Triangle2& uvTriangle,
		const Texture<Type>& textureSampler,
		const View<2, Type, Image_View>& image);

}

#include "pastel/gfx/draw_perspective_triangle.hpp"

#endif
