#ifndef PASTEL_DRAW_TEXTURED_ANNULUS_HPP
#define PASTEL_DRAW_TEXTURED_ANNULUS_HPP

#include "pastel/gfx/draw_textured_annulus.h"

#include "pastel/sys/vector_compare.h"
#include "pastel/sys/rectangle_tools.h"

namespace Pastel
{

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
		const ColorMixer& colorMixer)
	{
		// Let
		// u(x, y) = u_c + r(y) cos(alpha(x))
		// v(x, y) = v_c + r(y) sin(alpha(x))
		//
		// where
		// alpha(x) = minAlpha + [(x - xMin) / xDelta] deltaAlpha
		// r(y) = minRadius + [(y - yMin) / yDelta] deltaRadius
		//
		// where
		// xDelta = xMax - xMin
		// yDelta = yMax - yMin
		// deltaAlpha = maxAlpha - minAlpha
		// deltaRadius = maxRadius - minRadius
		//
		// Then:
		// (d/dx alpha)(x) = deltaAlpha / xDelta
		// (d/dy r)(y) = deltaRadius / yDelta
		//
		// (du/dx)(x, y) = -r(y) sin(alpha(x)) (d/dx alpha)(x)
		// (dv/dx)(x, y) =  r(y) cos(alpha(x)) (d/dx alpha)(x)
		// (du/dy)(x, y) = (d/dy r)(y) cos(alpha(x))
		// (dv/dy)(x, y) = (d/dy r)(y) sin(alpha(x))

		typedef View<2, Type, Image_View> Image;
		typedef typename Image::Cursor Cursor;

		if (box.empty() || allEqual(image.extent(), 0))
		{
			return;
		}

		const Vector2 boxExtent = box.extent();
		const real deltaRadius = annulusEndRadius - annulusStartRadius;
		const real deltaAngle = annulusEndAngle - annulusStartAngle;

		const real drDy = deltaRadius / boxExtent.y();
		const real daDx = deltaAngle / boxExtent.x();

		const Rectangle2 imageBox(
			toPixelSpanPoint(box.min().x()),
			toPixelSpanPoint(box.min().y()),
			toPixelSpanPoint(box.max().x()),
			toPixelSpanPoint(box.max().y()));
		const Rectangle2 imageWindow(
			Vector2i(0), image.extent());

		Rectangle2 clippedBox;
		if (intersect(imageBox, imageWindow, clippedBox))
		{
			const real daDx = deltaAngle / clippedBox.extent().x();
			const real drDy = deltaRadius / clippedBox.extent().y();

			const real aMin = annulusStartAngle +
				((clippedBox.min().x() + 0.5) - box.min().x()) * daDx;
			const real rMin = annulusStartRadius +
				((clippedBox.min().y() + 0.5) - box.min().y()) * drDy;

			Cursor yCursor(image.cursor(clippedBox.min()));

			const Vector<integer, 2> clippedBoxExtent =
				clippedBox.extent();

			real r = rMin;

			for (integer y = 0;y < clippedBoxExtent.y();++y)
			{
				Cursor xyCursor(yCursor);
				real a = aMin;

				for (integer x = 0;x < clippedBoxExtent.x();++x)
				{
					const real aCos = std::cos(a);
					const real aSin = std::sin(a);

					const Vector2 uv(
						annulusCenter + r * Vector2(aCos, aSin));

					const Vector2 duvDx(
						-r * aSin * daDx,
						r * aCos * daDx);

					const Vector2 duvDy(
						drDy * aCos,
						drDy * aSin);

					*xyCursor = colorMixer(*xyCursor, texture(uv, Matrix2(duvDx, duvDy)));

					a += daDx;
					xyCursor.xIncrement();
				}

				r += drDy;
				yCursor.yIncrement();
			}
		}
	}

}

#endif
