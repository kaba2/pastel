#ifndef PASTEL_DRAW_BOX_HPP
#define PASTEL_DRAW_BOX_HPP

#include "pastel/gfx/draw_box.h"
#include "pastel/gfx/assigncolormixer.h"
#include "pastel/gfx/color_texture.h"

#include "pastel/sys/rectangle.h"

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
		const Rectangle2 discreteBox(
			toPixelSpanPoint(box.min()), 
			toPixelSpanPoint(box.max()));

		Rectangle2 clippedBox;
		if (!intersect(discreteBox, 
			Rectangle2(Vector2i(0), image.extent()), 
			clippedBox))
		{
			return;
		}

		typedef typename Image_View::Cursor Cursor;

		Cursor yCursor = image.cursor(clippedBox.min());

		const integer width = clippedBox.extent().x();
		const integer height = clippedBox.extent().y();

		const Vector2 dp(
			inverse((real)width),
			inverse((real)height));

		const Matrix2 m(
			dp.x(), 0,
			0, dp.y());

		const Vector2 textureOrigin =
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
		const PASTEL_NO_DEDUCTION(Type)& color,
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
		const PASTEL_NO_DEDUCTION(Type)& color,
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
