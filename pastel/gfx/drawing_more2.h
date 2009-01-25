#ifndef PASTELGFX_DRAWING_MORE2_H
#define PASTELGFX_DRAWING_MORE2_H

#include "pastel/gfx/texture.h"
#include "pastel/sys/view.h"

namespace Pastel
{

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawProjectiveQuad(
		const Tuple<4, Point2>& quad,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image,
		const Tuple<4, Point2>& textureQuad,
		const ColorMixer& colorMixer);

	template <typename Type, typename Image_View>
	void drawProjectiveQuad(
		const Tuple<4, Point2>& quad,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image,
		const Tuple<4, Point2>& textureQuad);

	template <typename Type, typename Image_View>
	void drawProjectiveQuad(
		const Tuple<4, Point2>& quad,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image);

	//! Distorts an annulus in an image to a rectangular stripe.
	/*!
	Note that negative radius' also make sense. Also,
	end values need not be greater than start values.
	This can be used to alter the direction to which the
	distortion is done. One can also revolve multiple times
	around the annulus.
	*/

	template <typename Type, typename Image_View, typename ColorMixer>
	void distortAnnulusToAlignedBox(
		const Texture<Type>& texture,
		const Point2& annulusCenter,
		real annulusStartRadius,
		real annulusEndRadius,
		real annulusStartAngle,
		real annulusEndAngle, 
		const View<2, Type, Image_View>& image,
		const AlignedBox2& box,
		const ColorMixer& colorMixer);

}

#include "pastel/gfx/drawing_more2.hpp"

#endif
