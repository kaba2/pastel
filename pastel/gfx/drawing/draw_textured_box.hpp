#ifndef PASTELGFX_DRAW_TEXTURED_BOX_HPP
#define PASTELGFX_DRAW_TEXTURED_BOX_HPP

#include "pastel/gfx/drawing/draw_textured_box.h"
#include "pastel/gfx/color/colormixer/assign_colormixer.h"

#include "pastel/math/matrix/matrix.h"

#include "pastel/sys/intersect_alignedbox_alignedbox.h"
#include "pastel/sys/syscommon.h"

namespace Pastel
{

	template <
		typename Image_Element,
		typename Image_View,
		typename ColorMixer>
	void drawTexturedBox(
		const AlignedBox2& box,
		const Texture<Image_Element>& textureSampler,
		const View<2, Image_Element, Image_View>& image,
		const AlignedBox2& textureBox,
		const ColorMixer& colorMixer)
	{
		Vector2 delta = box.extent();

		AlignedBox2i discreteBox(
			toPixelSpanPoint(box.min()), 
			toPixelSpanPoint(box.max()));

		AlignedBox2i window(
			Vector2i(0), 
			image.extent());

		AlignedBox2i clippedBox;
		if (!intersect(discreteBox, window, clippedBox))
		{
			return;
		}

		Vector2 textureDelta = textureBox.extent();
		Vector2 duvDxy = textureDelta / delta;
		Vector2 uvMin = textureBox.min() + 

			((Vector2(discreteBox.min()) + 0.5) - box.min()) * duvDxy;

		Vector2 uv(uvMin);
		Vector2 duvDx(duvDxy.x(), 0);
		Vector2 duvDy(0, duvDxy.y());

		using Cursor = typename Image_View::Cursor;

		Cursor yCursor = image.cursor(clippedBox.min());

		integer width = clippedBox.extent()[0];
		integer height = clippedBox.extent()[1];

		for (integer y = 0; y < height;++y)
		{
			Cursor xyCursor = yCursor;

			uv.x() = uvMin.x();

			for (integer x = 0; x < width;++x)
			{

				*xyCursor = colorMixer(*xyCursor,
					textureSampler(uv, matrix2x2<real>(duvDx, duvDy)));

				uv.x() += duvDxy.x();
				xyCursor.xIncrement();
			}

			uv.y() += duvDxy.y();
			yCursor.yIncrement();
		}
	}

	template <
		typename Image_Element,
		typename Image_View>
	void drawTexturedBox(
		const AlignedBox2& box,
		const Texture<Image_Element>& textureSampler,
		const View<2, Image_Element, Image_View>& image,
		const AlignedBox2& textureBox)
	{
		drawTexturedBox(box, textureSampler, image, 
			textureBox, assignColorMixer<Image_Element>());
	}

	template <
		typename Image_Element,
		typename Image_View>
	void drawTexturedBox(
		const AlignedBox2& box,
		const Texture<Image_Element>& textureSampler,
		const View<2, Image_Element, Image_View>& image)
	{
		drawTexturedBox(box, textureSampler, 
			image, AlignedBox2(0, 0, 1, 1));
	}

}

#endif
