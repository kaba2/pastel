// Description: Drawing a box

#ifndef PASTEL_DRAW_BOX_H
#define PASTEL_DRAW_BOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"

#include "pastel/gfx/texture.h"

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Draws an axis-aligned rectangular area.

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawBox(
		const AlignedBox2& box,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer);

	//! Draws an axis-aligned rectangular area.

	template <typename Type, typename Image_View>
	void drawBox(
		const AlignedBox2& box,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image);

	//! Draws an axis-aligned rectangular area.
	/*!
	This is a convenience function that calls:
	*/
	template <typename Type, typename Image_View>
	void drawBox(
		const AlignedBox2& box,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image);

	//! Draws an axis-aligned rectangular area.

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawBox(
		const AlignedBox2& box,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer);
}

#include "pastel/gfx/draw_box.hpp"

#endif
