#ifndef PASTEL_DRAW_VIEW_HPP
#define PASTEL_DRAW_VIEW_HPP

#include "pastel/geometry/intersect_alignedbox_alignedbox.h"

#include "pastel/gfx/draw_view.h"
#include "pastel/gfx/assigncolormixer.h"

#include "pastel/sys/view_visit.h"
#include "pastel/sys/subview.h"

namespace Pastel
{

	namespace Detail_DrawView
	{

		template <typename ColorMixer>
		class Visitor
		{
		public:
			explicit Visitor(
				const ColorMixer& colorMixer)
				: colorMixer_(colorMixer)
			{
			}

			template <typename Texture, typename Image>
			void operator()(
				const Texture& texture,
				Image& image) const
			{
				image = colorMixer_(image, texture);
			}

		private:
			const ColorMixer& colorMixer_;
		};

	}

	template <int N,
		typename Texture_Element, typename Texture_View,
		typename Image_Element, typename Image_View,
		typename ColorMixer>
	void drawView(
		const ConstView<N, Texture_Element, Texture_View>& texture,
		const Vector<integer, N>& position,
		const View<N, Image_Element, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		const AlignedBox<integer, N> textureBox(
			position, position + texture.extent());
		const AlignedBox<integer, N> imageBox(
			Vector<integer, N>(0), image.extent());

		AlignedBox<integer, N> clippedTextureBox;
		if (!intersect(textureBox, imageBox, clippedTextureBox))
		{
			return;
		}

		Detail_DrawView::Visitor<ColorMixer> visitor(colorMixer);

		visit(constSubView(texture, clippedTextureBox - position),
			subView(image, clippedTextureBox), visitor);
	}

	template <int N,
		typename Texture_Element, typename Texture_View,
		typename Image_Element, typename Image_View>
	void drawView(
		const ConstView<N, Texture_Element, Texture_View>& texture,
		const Vector<integer, N>& position,
		const View<N, Image_Element, Image_View>& image)
	{
		drawView(texture, position, image, assignColorMixer<Image_Element>());
	}

}

#endif
