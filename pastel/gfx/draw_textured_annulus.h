// Description: Drawing a textured annulus

#ifndef PASTEL_DRAW_TEXTURED_ANNULUS_H
#define PASTEL_DRAW_TEXTURED_ANNULUS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/alignedbox.h"

#include "pastel/gfx/texture.h"

namespace Pastel
{

	//! Distorts an annulus in an image to a rectangular stripe.
	/*!
	Note that negative radii also make sense. Also,
	end values need not be greater than start values.
	This can be used to alter the direction to which the
	distortion is done. One can also revolve multiple times
	around the annulus.
	*/
	template <typename Type, typename Image_View, typename ColorMixer>
	void drawTexturedAnnulus(
		const Texture<Type>& texture,
		const Vector2& annulusCenter,
		real annulusStartRadius,
		real annulusEndRadius,
		real annulusStartAngle,
		real annulusEndAngle,
		const View<2, Type, Image_View>& image,
		const AlignedBox2& box,
		const ColorMixer& colorMixer);

}

#include "pastel/gfx/draw_textured_annulus.hpp"

#endif
