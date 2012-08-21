// Description: Drawing a quad in perspective
// Documentation: drawing.txt

#ifndef PASTEL_DRAW_PROJECTIVE_QUAD_H
#define PASTEL_DRAW_PROJECTIVE_QUAD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"
#include "pastel/sys/tuple.h"

#include "pastel/gfx/texture.h"

namespace Pastel
{

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawProjectiveQuad(
		const Tuple<Vector2, 4>& quad,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image,
		const Tuple<Vector2, 4>& textureQuad,
		const ColorMixer& colorMixer);

	template <typename Type, typename Image_View>
	void drawProjectiveQuad(
		const Tuple<Vector2, 4>& quad,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image,
		const Tuple<Vector2, 4>& textureQuad);

	template <typename Type, typename Image_View>
	void drawProjectiveQuad(
		const Tuple<Vector2, 4>& quad,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image);

}

#include "pastel/gfx/draw_projective_quad.hpp"

#endif
