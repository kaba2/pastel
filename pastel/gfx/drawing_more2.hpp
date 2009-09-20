#ifndef PASTEL_DRAWING_MORE2_HPP
#define PASTEL_DRAWING_MORE2_HPP

#include "pastel/gfx/drawing_more2.h"
#include "pastel/gfx/assigncolormixer.h"

#include "pastel/sys/view.h"

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
		const Matrix3 matrix = projectiveTransformation(textureQuad, quad);

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

	template <typename Type, typename Image_View, typename ColorMixer>
	void distortAnnulusToAlignedBox(
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
