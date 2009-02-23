/*!
\file
\brief Functions for drawing on 2d arrays.
*/

#ifndef PASTELGFX_DRAWING_H
#define PASTELGFX_DRAWING_H

#include "pastel/sys/array.h"
#include "pastel/sys/view.h"
#include "pastel/sys/rectangle.h"

#include "pastel/gfx/color.h"
#include "pastel/gfx/texture.h"

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/triangle.h"
#include "pastel/geometry/sphere.h"
#include "pastel/geometry/segment.h"

namespace Pastel
{

	/*!
	Example:
	Given a 2d horizontal line segment [xMin, xMax[ on row y,
	xMin, xMax real numbers, y an integer.
	Which pixels should be set? Answer:
	[toPixelSpanPoint(xMin), toPixelSpanPoint(xMax)[
	on row y.
	*/
	integer toPixelSpanPoint(real t);

	//! Draws a pixel.

	template <int N, typename Type, typename Image_View, typename ColorMixer>
	void drawPixel(
		const PASTEL_NO_DEDUCTION((Point<N, integer>))& position,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<N, Type, Image_View>& image,
		const ColorMixer& colorMixer);

	//! Draws a pixel.

	template <int N, typename Type, typename Image_View>
	void drawPixel(
		const PASTEL_NO_DEDUCTION((Point<N, integer>))& position,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<N, Type, Image_View>& image);

	//! Draws a line segment.

	template <typename Type, typename Image_View>
	void drawSegmentOld(
		const Segment2& segment,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image);

	//! Draws a line segment.

	template <typename Type, typename Image_View>
	void drawSegment(
		const Segment2& segment,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image);

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

	//! Draws an axis aligned rectangular area.

	template <typename Type, typename Image_View>
	void drawBox(
		const Rectangle2& box,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image);

	//! Changes the color of a connected region.
	/*!
	If xStart or yStart is out of range, nothing
	will happen. Otherwise the connected component
	that (xStart, yStart) is part of is
	recolored with the given color.
	*/
	template <typename Type, typename Image_View>
	void floodFill(
		integer xStart, integer yStart,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image);

	//! Draws a texture in its native size.

	template <int N,
		typename Texture_Element, typename Texture_View,
		typename Image_Element, typename Image_View,
		typename ColorMixer>
	void drawView(
		const ConstView<N, Texture_Element, Texture_View>& texture,
		const Point<N, integer>& position,
		const View<N, Image_Element, Image_View>& image,
		const ColorMixer& colorMixer);

	//! Draws a texture in its native size.
	/*!
	This function calls the more general
	drawView() function with simple assignment
	as the color mixer.
	*/

	template <int N,
		typename Texture_Element, typename Texture_View,
		typename Image_Element, typename Image_View>
	void drawView(
		const ConstView<N, Texture_Element, Texture_View>& texture,
		const Point<N, integer>& position,
		const View<N, Image_Element, Image_View>& image);

	//! Draws a textured axis aligned box.

	template <
		typename Image_Element,
		typename Image_View,
		typename ColorMixer>
	void drawTexturedBox(
		const AlignedBox2& box,
		const Texture<Image_Element>& textureSampler,
		const View<2, Image_Element, Image_View>& image,
		const AlignedBox2& textureBox,
		const ColorMixer& colorMixer);

	//! Draws a textured axis aligned box.
	/*!
	This is a shortcut to the more general
	'drawTexturedBox' function. It uses
	a simple assignment as the ColorMixer.
	*/

	template <
		typename Image_Element,
		typename Image_View>
	void drawTexturedBox(
		const AlignedBox2& box,
		const Texture<Image_Element>& textureSampler,
		const View<2, Image_Element, Image_View>& image,
		const AlignedBox2& textureBox);

	//! Draws a textured axis aligned box.
	/*!
	This is a shortcut to the more general
	'drawTexturedBox' function. It uses
	a simple assignment as the ColorMixer
	and the texture box (0, 0, 1, 1).
	*/

	template <
		typename Image_Element,
		typename Image_View>
	void drawTexturedBox(
		const AlignedBox2& box,
		const Texture<Image_Element>& textureSampler,
		const View<2, Image_Element, Image_View>& image);

}

#include "pastel/gfx/drawing_more.h"
#include "pastel/gfx/drawing_more2.h"

#include "pastel/gfx/drawing.hpp"

#endif
