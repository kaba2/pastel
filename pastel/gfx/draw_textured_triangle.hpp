#ifndef PASTEL_DRAW_TEXTURED_TRIANGLE_HPP
#define PASTEL_DRAW_TEXTURED_TRIANGLE_HPP

#include "pastel/gfx/draw_textured_triangle.h"
#include "pastel/gfx/assigncolormixer.h"

#include "pastel/geometry/plane.h"

#include "pastel/sys/syscommon.h"
#include "pastel/sys/smallset.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	namespace Detail_DrawTexturedTriangle
	{

		class TextureVertex
		{
		public:
			TextureVertex()
				: x_(0)
				, y_(0)
				, uv_(0)
			{
			}

			TextureVertex(
				const Vector2& position,
				const Vector2& uv,
				integer index)
				: x_(position[0])
				, y_(position[1])
				, uv_(uv)
				, index_(index)
			{
			}

			real x_;
			real y_;
			Vector2 uv_;
			integer index_;
		};

		class VerticalComparison
		{
		public:
			bool operator()(
				const TextureVertex& left,
				const TextureVertex& right) const
			{
				if (left.y_ != right.y_)
				{
					return left.y_ < right.y_;
				}

				if (left.x_ != right.x_)
				{
					return left.x_ < right.x_;
				}

				return left.index_ < right.index_;
			}
		};

		class HorizontalComparison
		{
		public:
			bool operator()(
				const TextureVertex& left,
				const TextureVertex& right) const
			{
				if (left.x_ != right.x_)
				{
					return left.x_ < right.x_;
				}

				if (left.y_ != right.y_)
				{
					return left.y_ < right.y_;
				}

				return left.index_ < right.index_;
			}
		};

	}

	template <typename Type,
		typename Image_View, typename ColorMixer>
	void drawTriangle(
		const Triangle2& triangle,
		const Triangle2& uvTriangle,
		const Texture<Type>& textureSampler,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		typedef Detail_DrawTexturedTriangle::TextureVertex Vertex;

		// Order vertices vertically.

		SmallSet<Vertex, Detail_DrawTexturedTriangle::VerticalComparison> yVertex;
		yVertex.reserve(3);

		yVertex.insert(Vertex(triangle[0], uvTriangle[0], 0));
		yVertex.insert(Vertex(triangle[1], uvTriangle[1], 1));
		yVertex.insert(Vertex(triangle[2], uvTriangle[2], 2));

		const Vertex& yMinVertex = yVertex[0];
		const Vertex& yMidVertex = yVertex[1];
		const Vertex& yMaxVertex = yVertex[2];

		// Order vertices horizontally.

		SmallSet<Vertex, Detail_DrawTexturedTriangle::HorizontalComparison> xVertex;

		xVertex.insert(Vertex(triangle[0], uvTriangle[0], 0));
		xVertex.insert(Vertex(triangle[1], uvTriangle[1], 1));
		xVertex.insert(Vertex(triangle[2], uvTriangle[2], 2));

		const Vertex& xMinVertex = xVertex[0];
		const Vertex& xMidVertex = xVertex[1];
		const Vertex& xMaxVertex = xVertex[2];

		// Cull and clip.

		integer xMin = toPixelSpanPoint(xMinVertex.x_);
		integer xMid = toPixelSpanPoint(xMidVertex.x_);
		integer xMax = toPixelSpanPoint(xMaxVertex.x_);

		integer yMin = toPixelSpanPoint(yMinVertex.y_);
		integer yMid = toPixelSpanPoint(yMidVertex.y_);
		integer yMax = toPixelSpanPoint(yMaxVertex.y_);

		const integer width = image.width();
		const integer height = image.height();

		if (xMax <= 0 || xMin >= width ||
			yMax <= 0 || yMin >= height)
		{
			return;
		}

		if (yMin < 0)
		{
			yMin = 0;
		}
		if (yMax > height)
		{
			yMax = height;
		}
		if (yMid < 0)
		{
			yMid = 0;
		}
		if (yMid > height)
		{
			yMid = height;
		}

		// The polygon spans the following pixel y ranges:
		// [yMin, yMid[
		// [yMid, yMax[

		// Compute delta's w.r.t. y.

		real xLeftDelta = 0;
		real yLeftDelta = 0;
		Vector2 uvLeftDelta;

		real xRightDelta = 0;
		real yRightDelta = 0;
		Vector2 uvRightDelta;

		const bool longLeftEdge =
			side(
			Vector2(yMidVertex.x_, yMidVertex.y_),
			Plane2(
			Vector2(yMinVertex.x_, yMinVertex.y_),
			cross(Vector2(yMaxVertex.x_, yMaxVertex.y_) -
			Vector2(yMinVertex.x_, yMinVertex.y_)))) < 0;

		if (longLeftEdge)
		{
			xLeftDelta = yMaxVertex.x_ - yMinVertex.x_;
			yLeftDelta = yMaxVertex.y_ - yMinVertex.y_;
			uvLeftDelta = yMaxVertex.uv_ - yMinVertex.uv_;

			xRightDelta = yMidVertex.x_ - yMinVertex.x_;
			yRightDelta = yMidVertex.y_ - yMinVertex.y_;
			uvRightDelta = yMidVertex.uv_ - yMinVertex.uv_;
		}
		else
		{
			xLeftDelta = yMidVertex.x_ - yMinVertex.x_;
			yLeftDelta = yMidVertex.y_ - yMinVertex.y_;
			uvLeftDelta = yMidVertex.uv_ - yMinVertex.uv_;

			xRightDelta = yMaxVertex.x_ - yMinVertex.x_;
			yRightDelta = yMaxVertex.y_ - yMinVertex.y_;
			uvRightDelta = yMaxVertex.uv_ - yMinVertex.uv_;
		}

		// Compute delta's w.r.t. x.

		real yBottomDelta = 0;
		real xBottomDelta = 0;
		Vector2 uvBottomDelta;

		real yTopDelta = 0;
		real xTopDelta = 0;
		Vector2 uvTopDelta;

		const bool longBottomEdge =
			side(
			Vector2(xMidVertex.x_, xMidVertex.y_),
			Plane2(
			Vector2(xMinVertex.x_, xMinVertex.y_),
			cross(Vector2(xMaxVertex.x_, xMaxVertex.y_) -
			Vector2(xMinVertex.x_, xMinVertex.y_)))) < 0;

		if (longBottomEdge)
		{
			yBottomDelta = xMaxVertex.y_ - xMinVertex.y_;
			xBottomDelta = xMaxVertex.x_ - xMinVertex.x_;
			uvBottomDelta = xMaxVertex.uv_ - xMinVertex.uv_;

			yTopDelta = xMidVertex.y_ - xMinVertex.y_;
			xTopDelta = xMidVertex.x_ - xMinVertex.x_;
			uvTopDelta = xMidVertex.uv_ - xMinVertex.uv_;
		}
		else
		{
			yBottomDelta = xMidVertex.y_ - xMinVertex.y_;
			xBottomDelta = xMidVertex.x_ - xMinVertex.x_;
			uvBottomDelta = xMidVertex.uv_ - xMinVertex.uv_;

			yTopDelta = xMaxVertex.y_ - xMinVertex.y_;
			xTopDelta = xMaxVertex.x_ - xMinVertex.x_;
			uvTopDelta = xMaxVertex.uv_ - xMinVertex.uv_;
		}

		// Compute boundary derivatives w.r.t. y.

		real dxLeftDy = xLeftDelta;
		Vector2 dUvLeftDy = uvLeftDelta;
		if (yLeftDelta != 0)
		{
			const real yLeftDeltaInv = inverse(yLeftDelta);

			dxLeftDy *= yLeftDeltaInv;
			dUvLeftDy *= yLeftDeltaInv;
		}

		real dxRightDy = xRightDelta;
		Vector2 dUvRightDy = uvRightDelta;
		if (yRightDelta != 0)
		{
			const real yRightDeltaInv = inverse(yRightDelta);

			dxRightDy *= yRightDeltaInv;
			dUvRightDy *= yRightDeltaInv;
		}

		// Compute boundary derivatives w.r.t. x.

		real dyBottomDx = yBottomDelta;
		Vector2 dUvBottomDx = uvBottomDelta;
		if (xBottomDelta != 0)
		{
			const real xBottomDeltaInv = inverse(xBottomDelta);

			dyBottomDx *= xBottomDeltaInv;
			dUvBottomDx *= xBottomDeltaInv;
		}

		real dyTopDx = yTopDelta;
		Vector2 dUvTopDx = uvTopDelta;
		if (xTopDelta != 0)
		{
			const real xTopDeltaInv = inverse(xTopDelta);

			dyTopDx *= xTopDeltaInv;
			dUvTopDx *= xTopDeltaInv;
		}

		// Offset the start scanline to pixel rows.

		const real yOffset = ((real)yMin + 0.5) - yMinVertex.y_;

		real xLeft = yMinVertex.x_ + dxLeftDy * yOffset;
		Vector2 uvLeft = yMinVertex.uv_ + dUvLeftDy * yOffset;

		real xRight = yMinVertex.x_ + dxRightDy * yOffset;
		Vector2 uvRight = yMinVertex.uv_ + dUvRightDy * yOffset;

		// Compute data derivatives w.r.t. x.

		Vector2 dUvDx;
		if (longLeftEdge)
		{
			const real yDelta = yMidVertex.y_ - yMinVertex.y_;

			const real dx =
				yMidVertex.x_ -
				(yMinVertex.x_ + dxLeftDy * yDelta);
			const Vector2 dUv =
				yMidVertex.uv_ -
				(yMinVertex.uv_ + dUvLeftDy * yDelta);

			const real invDx = inverse(dx);

			dUvDx = dUv * invDx;
		}
		else
		{
			const real yDelta = yMidVertex.y_ - yMinVertex.y_;

			const real dx =
				(yMinVertex.x_ + dxRightDy * yDelta) -
				yMidVertex.x_;
			const Vector2 dUv =
				(yMinVertex.uv_ + dUvRightDy * yDelta) -
				yMidVertex.uv_;

			const real invDx = inverse(dx);

			dUvDx = dUv * invDx;
		}

		// Compute data derivatives w.r.t. y.

		Vector2 dUvDy;
		if (longBottomEdge)
		{
			const real xDelta = xMidVertex.x_ - xMinVertex.x_;

			const real dy =
				xMidVertex.y_ -
				(xMinVertex.y_ + dyBottomDx * xDelta);
			const Vector2 dUv =
				xMidVertex.uv_ -
				(xMinVertex.uv_ + dUvBottomDx * xDelta);

			const real invDy = inverse(dy);

			dUvDy = dUv * invDy;
		}
		else
		{
			const real xDelta = xMidVertex.x_ - xMinVertex.x_;

			const real dy =
				(xMinVertex.y_ + dyTopDx * xDelta) -
				xMidVertex.y_;
			const Vector2 dUv =
				(xMinVertex.uv_ + dUvTopDx * xDelta) -
				xMidVertex.uv_;

			const real invDy = inverse(dy);

			dUvDy = dUv * invDy;
		}

		// Scan convert the bottom part.

		for (integer y = yMin;y < yMid;++y)
		{
			const integer xBegin = std::max(toPixelSpanPoint(xLeft), 0);
			const integer xEnd = std::min(toPixelSpanPoint(xRight), width);

			if (xEnd - xBegin > 0)
			{
				const real xOffset = ((real)xBegin + 0.5) - xLeft;

				Vector2 uv = uvLeft + dUvDx * xOffset;

				for (integer x = xBegin;x < xEnd;++x)
				{
					image(x, y) =
						colorMixer(image(x, y),
						textureSampler(uv, Matrix2(dUvDx, dUvDy)));

					uv += dUvDx;
				}
			}

			xLeft += dxLeftDy;
			uvLeft += dUvLeftDy;

			xRight += dxRightDy;
			uvRight += dUvRightDy;
		}

		// Prepare for scan converting the upper part.
		// Only one of the side changes direction.

		if (longLeftEdge)
		{
			yRightDelta = yMaxVertex.y_ - yMidVertex.y_;

			xRightDelta = yMaxVertex.x_ - yMidVertex.x_;
			uvRightDelta = yMaxVertex.uv_ - yMidVertex.uv_;

			dxRightDy = xRightDelta;
			dUvRightDy = uvRightDelta;
			if (yRightDelta != 0)
			{
				const real yRightDeltaInv = inverse(yRightDelta);

				dxRightDy *= yRightDeltaInv;
				dUvRightDy *= yRightDeltaInv;
			}

			// Offset the scanline to pixel rows.

			const real yOffset = ((real)yMid + 0.5) - yMidVertex.y_;

			xRight = yMidVertex.x_ + dxRightDy * yOffset;
			uvRight = yMidVertex.uv_ + dUvRightDy * yOffset;
		}
		else
		{
			yLeftDelta = yMaxVertex.y_ - yMidVertex.y_;

			xLeftDelta = yMaxVertex.x_ - yMidVertex.x_;
			uvLeftDelta = yMaxVertex.uv_ - yMidVertex.uv_;

			dxLeftDy = xLeftDelta;
			dUvLeftDy = uvLeftDelta;
			if (yLeftDelta != 0)
			{
				const real yLeftDeltaInv = inverse(yLeftDelta);

				dxLeftDy *= yLeftDeltaInv;
				dUvLeftDy *= yLeftDeltaInv;
			}

			// Offset the scanline to pixel rows.

			const real yOffset = ((real)yMid + 0.5) - yMidVertex.y_;

			xLeft = yMidVertex.x_ + dxLeftDy * yOffset;
			uvLeft = yMidVertex.uv_ + dUvLeftDy * yOffset;
		}

		// Scan convert the top part.

		for (integer y = yMid;y < yMax;++y)
		{
			const integer xBegin = std::max(toPixelSpanPoint(xLeft), 0);
			const integer xEnd = std::min(toPixelSpanPoint(xRight), width);

			if (xEnd - xBegin > 0)
			{
				const real xOffset = ((real)xBegin + 0.5) - xLeft;

				Vector2 uv = uvLeft + dUvDx * xOffset;

				for (integer x = xBegin;x < xEnd;++x)
				{
					image(x, y) =
						colorMixer(image(x, y),
						textureSampler(uv, Matrix2(dUvDx, dUvDy)));

					uv += dUvDx;
				}
			}

			xLeft += dxLeftDy;
			uvLeft += dUvLeftDy;

			xRight += dxRightDy;
			uvRight += dUvRightDy;
		}
	}

	template <typename Type,
		typename Image_View>
	void drawTriangle(
		const Triangle2& triangle,
		const Triangle2& uvTriangle,
		const Texture<Type>& textureSampler,
		const View<2, Type, Image_View>& image)
	{
		drawTriangle(triangle, uvTriangle,
			textureSampler, image, assignColorMixer<Type>());
	}

}

#endif
