#ifndef PASTELGFX_DRAW_PROJECTIVE_QUAD_HPP
#define PASTELGFX_DRAW_PROJECTIVE_QUAD_HPP

#include "pastel/gfx/drawing/draw_projective_quad.h"
#include "pastel/gfx/color/colormixer/assign_colormixer.h"

#include "pastel/geometry/planar_projection.h"

namespace Pastel
{

	template <typename Type,
		typename Image_View, typename ColorMixer>
	void drawProjectiveQuad(
		const Tuple<Vector2, 4>& quad,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image,
		const Tuple<Vector2, 4>& textureQuad,
		const ColorMixer& colorMixer)
	{
		Matrix<real> matrix = projectiveTransformation(textureQuad, quad);

		const Vector3 a(wDivide(evaluate(extend(textureQuad[0], 1) * matrix)));
		const Vector3 b(wDivide(evaluate(extend(textureQuad[1], 1) * matrix)));
		const Vector3 c(wDivide(evaluate(extend(textureQuad[2], 1) * matrix)));
		const Vector3 d(wDivide(evaluate(extend(textureQuad[3], 1) * matrix)));

		drawTriangle(
			Triangle3(a, b, d),
			Triangle2(textureQuad[0], textureQuad[1], textureQuad[3]),
			texture,
			image);

		drawTriangle(
			Triangle3(b, c, d),
			Triangle2(textureQuad[1], textureQuad[2], textureQuad[3]),
			texture,
			image);
	}

	template <typename Type, typename Image_View>
	void drawProjectiveQuad(
		const Tuple<Vector2, 4>& quad,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image,
		const Tuple<Vector2, 4>& textureQuad)
	{
		Pastel::drawProjectiveQuad(quad,
			texture, image, textureQuad,
			assignColorMixer<Type>());
	}

	template <typename Type, typename Image_View>
	void drawProjectiveQuad(
		const Tuple<Vector2, 4>& quad,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image)
	{
		Pastel::drawProjectiveQuad(quad,
			texture, image,
			Tuple<Vector2, 4>(
			Vector2(0, 0),
			Vector2(1, 0),
			Vector2(1, 1),
			Vector2(0, 1)),
			assignColorMixer<Type>());
	}

}

#endif
