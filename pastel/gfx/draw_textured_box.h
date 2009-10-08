// Description: Drawing a textured box

#ifndef DRAW_TEXTURED_BOX_H
#define DRAW_TEXTURED_BOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"

#include "pastel/gfx/texture.h"

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Draws a textured axis-aligned box.

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

	//! Draws a textured axis-aligned box.
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

	//! Draws a textured axis-aligned box.
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

#include "pastel/gfx/draw_textured_box.hpp"

#endif
