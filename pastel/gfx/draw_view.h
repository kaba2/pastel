// Description: Drawing a view
// Documentation: drawing.txt

#ifndef PASTELGFX_DRAW_VIEW_H
#define PASTELGFX_DRAW_VIEW_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view/view.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Draws a texture in its native size.

	template <int N,
		typename Texture_Element, typename Texture_View,
		typename Image_Element, typename Image_View,
		typename ColorMixer>
	void drawView(
		const ConstView<N, Texture_Element, Texture_View>& texture,
		const Vector<integer, N>& position,
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
		const Vector<integer, N>& position,
		const View<N, Image_Element, Image_View>& image);

}

#include "pastel/gfx/draw_view.hpp"

#endif
