#ifndef PASTEL_DRAWING_MORE_H
#define PASTEL_DRAWING_MORE_H

#include "pastel/gfx/drawing.h"

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


#endif
