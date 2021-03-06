#ifndef PASTELGFX_DRAW_BOX_HPP
#define PASTELGFX_DRAW_BOX_HPP

#include "pastel/gfx/drawing/draw_box.h"
#include "pastel/gfx/color/colormixer/assign_colormixer.h"
#include "pastel/gfx/texture/color_texture.h"

#include "pastel/geometry/intersect/intersect_alignedbox_alignedbox.h"

#include "pastel/sys/alignedbox.h"
#include "pastel/sys/syscommon.h"

namespace Pastel
{

	template <typename Type, typename Image_View>
	void drawBox(
		const AlignedBox2& box,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image)
	{
		Pastel::drawBox(box, texture, image, assignColorMixer<Type>());
	}

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawBox(
		const AlignedBox2& box,
		const Texture<Type>& texture,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		AlignedBox2i discreteBox(
			toPixelSpanPoint(box.min()), 
			toPixelSpanPoint(box.max()));

		AlignedBox2i clippedBox;
		if (!intersect(discreteBox, 
			AlignedBox2i(Vector2i(0), image.extent()), 
			clippedBox))
		{
			return;
		}

		using Cursor = typename Image_View::Cursor;

		Cursor yCursor = image.cursor(clippedBox.min());

		integer width = clippedBox.extent()[0];
		integer height = clippedBox.extent()[1];

		Vector2 dp(
			inverse((dreal)width),
			inverse((dreal)height));

		Matrix<dreal, 2, 2> m = 
			matrix2x2<dreal>(
			dp.x(), 0,
			0, dp.y());

		Vector2 textureOrigin =
			((Vector2(clippedBox.min()) + 0.5) - box.min()) * dp;

		Vector2 p = textureOrigin;
		for (integer y = 0;y < height;++y)
		{
			Cursor xyCursor = yCursor;
			p.x() = textureOrigin.x();

			for (integer x = 0;x < width;++x)
			{
				*xyCursor = colorMixer(*xyCursor, texture(p, m));
				xyCursor.xIncrement();

				p.x() += dp.x();
			}

			yCursor.yIncrement();
			p.y() += dp.y();
		}
	}

	//! Draws an axis-aligned rectangular area.

	template <typename Type, typename Image_View>
	void drawBox(
		const AlignedBox2& box,
		const NoDeduction<Type>& color,
		const View<2, Type, Image_View>& image)
	{
		Pastel::drawBox(
			box, 
			colorTexture<Type, 2>(color),
			image);
	}

	//! Draws an axis-aligned rectangular area.

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawBox(
		const AlignedBox2& box,
		const NoDeduction<Type>& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		Pastel::drawBox(
			box, 
			colorTexture<Type, 2>(color),
			image,
			colorMixer);
	}

}

#endif
