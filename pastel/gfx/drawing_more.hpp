#ifndef PASTEL_DRAWING_MORE_HPP
#define PASTEL_DRAWING_MORE_HPP

#include "pastel/gfx/drawing.hpp"

namespace Pastel
{

	namespace Detail_SolidTriangle
	{

		class Vertex
		{
		public:
			Vertex()
				: position_()
				, index_(0)
			{
			}

			Vertex(
				const Point2& position,
				integer index)
				: position_(position)
				, index_(index)
			{
			}

			bool operator<(const Vertex& that) const
			{
				if (position_.y() != that.position_.y())
				{
					return position_.y() < that.position_.y();
				}

				if (position_.x() != that.position_.x())
				{
					return position_.x() < that.position_.x();
				}

				return index_ < that.index_;
			}

			Point2 position_;
			integer index_;
		};

	}


	template <typename Type, typename Image_View, typename ColorMixer>
	void drawTriangle(
		const Triangle2& triangle,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		typedef Detail_SolidTriangle::Vertex Vertex;

		SmallSet<Vertex> vertex;
		vertex.reserve(3);

		// Sort the triangle vertices lexicographically.
		vertex.insert(Vertex(triangle[0], 0));
		vertex.insert(Vertex(triangle[1], 1));
		vertex.insert(Vertex(triangle[2], 2));

		const bool longLeftEdge = side(vertex[1].position_,
			Plane2(vertex[0].position_,
			cross(vertex[2].position_ - vertex[0].position_))) < 0;

		integer yBottom = toPixelSpanPoint(vertex[0].position_.y());
		integer yMiddle = toPixelSpanPoint(vertex[1].position_.y());
		integer yTop = toPixelSpanPoint(vertex[2].position_.y());

		// Cull

		if (yTop <= 0 || yBottom >= image.height())
		{
			return;
		}

		// Clip y.

		if (yBottom < 0)
		{
			yBottom = 0;
		}
		if (yTop > image.height())
		{
			yTop = image.height();
		}
		if (yMiddle < 0)
		{
			yMiddle = 0;
		}
		if (yMiddle > image.height())
		{
			yMiddle = image.height();
		}

		// The polygon spans the following pixel y ranges:
		// [yBottom, yMiddle[
		// [yMiddle, yTop[

		// Prepare for scan converting
		// the lower part.

		Vector2 leftDelta;
		Vector2 rightDelta;

		if (longLeftEdge)
		{
			leftDelta = vertex[2].position_ - vertex[0].position_;
			rightDelta = vertex[1].position_ - vertex[0].position_;
		}
		else
		{
			leftDelta = vertex[1].position_ - vertex[0].position_;
			rightDelta = vertex[2].position_ - vertex[0].position_;
		}

		real xLeftAdd = leftDelta.x();
		if (leftDelta.y() != 0)
		{
			xLeftAdd /= leftDelta.y();
		}
		real xLeft = vertex[0].position_.x() +
			xLeftAdd * (((real)yBottom + 0.5) - vertex[0].position_.y());

		real xRightAdd = rightDelta.x();
		if (rightDelta.y() != 0)
		{
			xRightAdd /= rightDelta.y();
		}
		real xRight = vertex[0].position_.x() +
			xRightAdd * (((real)yBottom + 0.5) - vertex[0].position_.y());

		// Scan convert the bottom part.

		for (integer y = yBottom;y < yMiddle;++y)
		{
			drawHorizontalLine(xLeft, y, xRight, color, image, colorMixer);

			xLeft += xLeftAdd;
			xRight += xRightAdd;
		}

		// Prepare for scan converting the upper part.
		// Only one of the side changes direction.

		if (longLeftEdge)
		{
			rightDelta = vertex[2].position_ - vertex[1].position_;
			xRightAdd = rightDelta.x();
			if (rightDelta.y() != 0)
			{
				xRightAdd /= rightDelta.y();
			}
			xRight = vertex[1].position_.x() +
				xRightAdd * (((real)yMiddle + 0.5) - vertex[1].position_.y());
		}
		else
		{
			leftDelta = vertex[2].position_ - vertex[1].position_;
			xLeftAdd = leftDelta.x();
			if (leftDelta.y() != 0)
			{
				xLeftAdd /= leftDelta.y();
			}
			xLeft = vertex[1].position_.x() +
				xLeftAdd * (((real)yMiddle + 0.5) - vertex[1].position_.y());
		}

		// Scan convert the upper part

		for (integer y = yMiddle;y < yTop;++y)
		{
			drawHorizontalLine(xLeft, y, xRight, color, image, colorMixer);

			xLeft += xLeftAdd;
			xRight += xRightAdd;
		}
	}

	template <typename Type, typename Image_View>
	void drawTriangle(
		const Triangle2& triangle,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image)
	{
		drawTriangle(triangle, color, image, assignColorMixer<Type>());
	}

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
				const Point2& position,
				const Point2& uv,
				integer index)
				: x_(position[0])
				, y_(position[1])
				, uv_(uv)
				, index_(index)
			{
			}

			real x_;
			real y_;
			Point2 uv_;
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
			Point2(yMidVertex.x_, yMidVertex.y_),
			Plane2(
			Point2(yMinVertex.x_, yMinVertex.y_),
			cross(Point2(yMaxVertex.x_, yMaxVertex.y_) -
			Point2(yMinVertex.x_, yMinVertex.y_)))) < 0;

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
			Point2(xMidVertex.x_, xMidVertex.y_),
			Plane2(
			Point2(xMinVertex.x_, xMinVertex.y_),
			cross(Point2(xMaxVertex.x_, xMaxVertex.y_) -
			Point2(xMinVertex.x_, xMinVertex.y_)))) < 0;

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
		Point2 uvLeft = yMinVertex.uv_ + dUvLeftDy * yOffset;

		real xRight = yMinVertex.x_ + dxRightDy * yOffset;
		Point2 uvRight = yMinVertex.uv_ + dUvRightDy * yOffset;

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

				Point2 uv = uvLeft + dUvDx * xOffset;

				for (integer x = xBegin;x < xEnd;++x)
				{
					image(x, y) =
						colorMixer(image(x, y),
						textureSampler(uv, dUvDx, dUvDy));

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

				Point2 uv = uvLeft + dUvDx * xOffset;

				for (integer x = xBegin;x < xEnd;++x)
				{
					image(x, y) =
						colorMixer(image(x, y),
						textureSampler(uv, dUvDx, dUvDy));

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

	namespace Detail_DrawPerspectiveTriangle
	{

		class TextureVertex
		{
		public:
			TextureVertex()
				: x_(0)
				, y_(0)
				, t_()
				, uv_(0)
			{
			}

			TextureVertex(
				const Point3& position,
				const Point2& uv,
				integer index)
				: x_(position[0])
				, y_(position[1])
				, t_(inverse(position[2]))
				, uv_(uv[0] / position[2], uv[1] / position[2])
				, index_(index)
			{
			}

			real x_;
			real y_;

			typedef Point1 tPoint;
			typedef Vector1 tVector;

			tPoint t_;
			Point2 uv_;
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
		const Triangle3& triangle,
		const Triangle2& uvTriangle,
		const Texture<Type>& textureSampler,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		// The triangle is scan converted from bottom to top
		// with horizontal scanlines.
		// The left and right extrema can be tracked
		// by simple linear interpolation.
		// Except for x,y, and z, the data on the triangle
		// must be interpolated taking perspective into account.
		// This means that you must interpolate in the
		// homogeneous space (u / w, v / w , 1 / w) rather
		// than in (u, v).

		// The sampling points of pixels
		// are at half-integers. For example,
		// pixel (0, 0) has its sampling point
		// at (0.5, 0.5).
		// The following parts are not considered
		// to be part of the triangle:
		// - the upper right edge
		// - the lower right edge
		// - the top vertex
		// This way triangles with userared vertices
		// can be renderer without overlap.

		// Interpolations have been divided into the following
		// classes:
		//
		// 1) y
		// 2) x
		// 3) t = {1 / w)
		// 4) uv
		//
		// The values in 't' are those values that are simply
		// linearly interpolated with no need for perspective
		// correction.
		//
		// The values in 'uv' are those values requiring perspective
		// correction. The values are interpolated in a homogeneous
		// space.

		typedef Detail_DrawPerspectiveTriangle::TextureVertex Vertex;
		typedef Vertex::tPoint tPoint;
		typedef Vertex::tVector tVector;

		// Order vertices vertically.

		SmallSet<Vertex, Detail_DrawPerspectiveTriangle::VerticalComparison> yVertex;
		yVertex.reserve(3);

		yVertex.insert(Vertex(triangle[0], uvTriangle[0], 0));
		yVertex.insert(Vertex(triangle[1], uvTriangle[1], 1));
		yVertex.insert(Vertex(triangle[2], uvTriangle[2], 2));

		const Vertex& yMinVertex = yVertex[0];
		const Vertex& yMidVertex = yVertex[1];
		const Vertex& yMaxVertex = yVertex[2];

		// Order vertices horizontally.

		SmallSet<Vertex, Detail_DrawPerspectiveTriangle::HorizontalComparison> xVertex;

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
		tVector tLeftDelta;

		real xRightDelta = 0;
		real yRightDelta = 0;
		Vector2 uvRightDelta;
		tVector tRightDelta;

		const bool longLeftEdge =
			side(
			Point2(yMidVertex.x_, yMidVertex.y_),
			Plane2(
			Point2(yMinVertex.x_, yMinVertex.y_),
			cross(Point2(yMaxVertex.x_, yMaxVertex.y_) -
			Point2(yMinVertex.x_, yMinVertex.y_)))) < 0;

		if (longLeftEdge)
		{
			xLeftDelta = yMaxVertex.x_ - yMinVertex.x_;
			yLeftDelta = yMaxVertex.y_ - yMinVertex.y_;
			uvLeftDelta = yMaxVertex.uv_ - yMinVertex.uv_;
			tLeftDelta = yMaxVertex.t_ - yMinVertex.t_;

			xRightDelta = yMidVertex.x_ - yMinVertex.x_;
			yRightDelta = yMidVertex.y_ - yMinVertex.y_;
			uvRightDelta = yMidVertex.uv_ - yMinVertex.uv_;
			tRightDelta = yMidVertex.t_ - yMinVertex.t_;
		}
		else
		{
			xLeftDelta = yMidVertex.x_ - yMinVertex.x_;
			yLeftDelta = yMidVertex.y_ - yMinVertex.y_;
			uvLeftDelta = yMidVertex.uv_ - yMinVertex.uv_;
			tLeftDelta = yMidVertex.t_ - yMinVertex.t_;

			xRightDelta = yMaxVertex.x_ - yMinVertex.x_;
			yRightDelta = yMaxVertex.y_ - yMinVertex.y_;
			uvRightDelta = yMaxVertex.uv_ - yMinVertex.uv_;
			tRightDelta = yMaxVertex.t_ - yMinVertex.t_;
		}

		// Compute delta's w.r.t. x.

		real yBottomDelta = 0;
		real xBottomDelta = 0;
		Vector2 uvBottomDelta;
		tVector tBottomDelta;

		real yTopDelta = 0;
		real xTopDelta = 0;
		Vector2 uvTopDelta;
		tVector tTopDelta;

		const bool longBottomEdge =
			side(
			Point2(xMidVertex.x_, xMidVertex.y_),
			Plane2(
			Point2(xMinVertex.x_, xMinVertex.y_),
			cross(Point2(xMaxVertex.x_, xMaxVertex.y_) -
			Point2(xMinVertex.x_, xMinVertex.y_)))) < 0;

		if (longBottomEdge)
		{
			yBottomDelta = xMaxVertex.y_ - xMinVertex.y_;
			xBottomDelta = xMaxVertex.x_ - xMinVertex.x_;
			uvBottomDelta = xMaxVertex.uv_ - xMinVertex.uv_;
			tBottomDelta = xMaxVertex.t_ - xMinVertex.t_;

			yTopDelta = xMidVertex.y_ - xMinVertex.y_;
			xTopDelta = xMidVertex.x_ - xMinVertex.x_;
			uvTopDelta = xMidVertex.uv_ - xMinVertex.uv_;
			tTopDelta = xMidVertex.t_ - xMinVertex.t_;
		}
		else
		{
			yBottomDelta = xMidVertex.y_ - xMinVertex.y_;
			xBottomDelta = xMidVertex.x_ - xMinVertex.x_;
			uvBottomDelta = xMidVertex.uv_ - xMinVertex.uv_;
			tBottomDelta = xMidVertex.t_ - xMinVertex.t_;

			yTopDelta = xMaxVertex.y_ - xMinVertex.y_;
			xTopDelta = xMaxVertex.x_ - xMinVertex.x_;
			uvTopDelta = xMaxVertex.uv_ - xMinVertex.uv_;
			tTopDelta = xMaxVertex.t_ - xMinVertex.t_;
		}

		// Compute boundary derivatives w.r.t. y.

		real dxLeftDy = xLeftDelta;
		Vector2 dUvLeftDy = uvLeftDelta;
		tVector dtLeftDy = tLeftDelta;
		if (yLeftDelta != 0)
		{
			const real yLeftDeltaInv = inverse(yLeftDelta);

			dxLeftDy *= yLeftDeltaInv;
			dUvLeftDy *= yLeftDeltaInv;
			dtLeftDy *= yLeftDeltaInv;
		}

		real dxRightDy = xRightDelta;
		Vector2 dUvRightDy = uvRightDelta;
		tVector dtRightDy = tRightDelta;
		if (yRightDelta != 0)
		{
			const real yRightDeltaInv = inverse(yRightDelta);

			dxRightDy *= yRightDeltaInv;
			dUvRightDy *= yRightDeltaInv;
			dtRightDy *= yRightDeltaInv;
		}

		// Compute boundary derivatives w.r.t. x.

		real dyBottomDx = yBottomDelta;
		Vector2 dUvBottomDx = uvBottomDelta;
		tVector dtBottomDx = tBottomDelta;
		if (xBottomDelta != 0)
		{
			const real xBottomDeltaInv = inverse(xBottomDelta);

			dyBottomDx *= xBottomDeltaInv;
			dUvBottomDx *= xBottomDeltaInv;
			dtBottomDx *= xBottomDeltaInv;
		}

		real dyTopDx = yTopDelta;
		Vector2 dUvTopDx = uvTopDelta;
		tVector dtTopDx = tTopDelta;
		if (xTopDelta != 0)
		{
			const real xTopDeltaInv = inverse(xTopDelta);

			dyTopDx *= xTopDeltaInv;
			dUvTopDx *= xTopDeltaInv;
			dtTopDx *= xTopDeltaInv;
		}

		// Offset the start scanline to pixel rows.

		const real yOffset = ((real)yMin + 0.5) - yMinVertex.y_;

		real xLeft = yMinVertex.x_ + dxLeftDy * yOffset;
		Point2 uvLeft = yMinVertex.uv_ + dUvLeftDy * yOffset;
		tPoint tLeft = yMinVertex.t_ + dtLeftDy * yOffset;

		real xRight = yMinVertex.x_ + dxRightDy * yOffset;
		Point2 uvRight = yMinVertex.uv_ + dUvRightDy * yOffset;
		tPoint tRight = yMinVertex.t_ + dtRightDy * yOffset;

		// Compute data derivatives w.r.t. x.

		Vector2 dUvDx;
		tVector dtDx;
		if (longLeftEdge)
		{
			const real yDelta = yMidVertex.y_ - yMinVertex.y_;

			const real dx =
				yMidVertex.x_ -
				(yMinVertex.x_ + dxLeftDy * yDelta);
			const Vector2 dUv =
				yMidVertex.uv_ -
				(yMinVertex.uv_ + dUvLeftDy * yDelta);
			const tVector dt =
				yMidVertex.t_ -
				(yMinVertex.t_ + dtLeftDy * yDelta);

			const real invDx = inverse(dx);

			dUvDx = dUv * invDx;
			dtDx = dt * invDx;
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
			const tVector dt =
				(yMinVertex.t_ + dtRightDy * yDelta) -
				yMidVertex.t_;

			const real invDx = inverse(dx);

			dUvDx = dUv * invDx;
			dtDx = dt * invDx;
		}

		// Compute data derivatives w.r.t. y.

		Vector2 dUvDy;
		tVector dtDy;
		if (longBottomEdge)
		{
			const real xDelta = xMidVertex.x_ - xMinVertex.x_;

			const real dy =
				xMidVertex.y_ -
				(xMinVertex.y_ + dyBottomDx * xDelta);
			const Vector2 dUv =
				xMidVertex.uv_ -
				(xMinVertex.uv_ + dUvBottomDx * xDelta);
			const tVector dt =
				xMidVertex.t_ -
				(xMinVertex.t_ + dtBottomDx * xDelta);

			const real invDy = inverse(dy);

			dUvDy = dUv * invDy;
			dtDy = dt * invDy;
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
			const tVector dt =
				(xMinVertex.t_ + dtTopDx * xDelta) -
				xMidVertex.t_;

			const real invDy = inverse(dy);

			dUvDy = dUv * invDy;
			dtDy = dt * invDy;
		}

		// Compute 2nd order data derivatives for
		// the homogeneous variables.

		// d2sDxDx = (d/dx)(d/dx)s = d^2s/dx^2
		const Vector2 d2sDxDx = dUvDx * (dUvDx + dtDx[0]);
		// d2sDyDx = (d/dx)(d/dy)s = d^2s/(dx dy)
		const Vector2 d2sDyDx = dUvDy * (dUvDy + dtDy[0]);

		// Scan convert the bottom part.

		for (integer y = yMin;y < yMid;++y)
		{
			const integer xBegin = std::max(toPixelSpanPoint(xLeft), 0);
			const integer xEnd = std::min(toPixelSpanPoint(xRight), width);

			if (xEnd - xBegin > 0)
			{
				const real xOffset = ((real)xBegin + 0.5) - xLeft;

				Point2 uv = uvLeft + dUvDx * xOffset;
				tPoint t = tLeft + dtDx * xOffset;

				const real& uh = uv[0];
				const real& vh = uv[1];
				const real& wh = t[0];

				// (d/dx) u(x, y)
				// = (d/dx) (u(x, y) / w(x, y)) * w(x, y)
				// = [(d/dx) (u(x, y) / w(x, y))] * w(x, y) + (u(x, y) / w(x, y)) * [(d/dx) w(x, y)]

				// (d/dx) w(x, y) = -[(d/dx) (1 / w(x, y))] w^2(x, y)

				for (integer x = xBegin;x < xEnd;++x)
				{
					const real w = inverse(wh);

					const real dwDx = -dtDx[0] * w * w;
					const real dwDy = -dtDy[0] * w * w;

					Vector2 dsDx = dUvDx * w + asVector(uv) * dwDx;
					Vector2 dsDy = dUvDy * w + asVector(uv) * dwDy;

					image(x, y) =
						colorMixer(image(x, y),
						textureSampler(Point2(asVector(uv) * w), dsDx, dsDy));

					uv += dUvDx;
					t += dtDx;

					//dsDx += d2sDxDx;
					//dsDy += d2sDyDx;
				}
			}

			xLeft += dxLeftDy;
			uvLeft += dUvLeftDy;
			tLeft += dtLeftDy;

			xRight += dxRightDy;
			uvRight += dUvRightDy;
			tRight += dtRightDy;
		}

		// Prepare for scan converting the upper part.
		// Only one of the side changes direction.

		if (longLeftEdge)
		{
			yRightDelta = yMaxVertex.y_ - yMidVertex.y_;

			xRightDelta = yMaxVertex.x_ - yMidVertex.x_;
			uvRightDelta = yMaxVertex.uv_ - yMidVertex.uv_;
			tRightDelta = yMaxVertex.t_ - yMidVertex.t_;

			dxRightDy = xRightDelta;
			dUvRightDy = uvRightDelta;
			dtRightDy = tRightDelta;
			if (yRightDelta != 0)
			{
				const real yRightDeltaInv = inverse(yRightDelta);

				dxRightDy *= yRightDeltaInv;
				dUvRightDy *= yRightDeltaInv;
				dtRightDy *= yRightDeltaInv;
			}

			// Offset the scanline to pixel rows.

			const real yOffset = ((real)yMid + 0.5) - yMidVertex.y_;

			xRight = yMidVertex.x_ + dxRightDy * yOffset;
			uvRight = yMidVertex.uv_ + dUvRightDy * yOffset;
			tRight = yMidVertex.t_ + dtRightDy * yOffset;
		}
		else
		{
			yLeftDelta = yMaxVertex.y_ - yMidVertex.y_;

			xLeftDelta = yMaxVertex.x_ - yMidVertex.x_;
			uvLeftDelta = yMaxVertex.uv_ - yMidVertex.uv_;
			tLeftDelta = yMaxVertex.t_ - yMidVertex.t_;

			dxLeftDy = xLeftDelta;
			dUvLeftDy = uvLeftDelta;
			dtLeftDy = tLeftDelta;
			if (yLeftDelta != 0)
			{
				const real yLeftDeltaInv = inverse(yLeftDelta);

				dxLeftDy *= yLeftDeltaInv;
				dUvLeftDy *= yLeftDeltaInv;
				dtLeftDy *= yLeftDeltaInv;
			}

			// Offset the scanline to pixel rows.

			const real yOffset = ((real)yMid + 0.5) - yMidVertex.y_;

			xLeft = yMidVertex.x_ + dxLeftDy * yOffset;
			uvLeft = yMidVertex.uv_ + dUvLeftDy * yOffset;
			tLeft = yMidVertex.t_ + dtLeftDy * yOffset;
		}

		// Scan convert the top part.

		for (integer y = yMid;y < yMax;++y)
		{
			const integer xBegin = std::max(toPixelSpanPoint(xLeft), 0);
			const integer xEnd = std::min(toPixelSpanPoint(xRight), width);

			if (xEnd - xBegin > 0)
			{
				const real xOffset = ((real)xBegin + 0.5) - xLeft;

				Point2 uv = uvLeft + dUvDx * xOffset;
				tPoint t = tLeft + dtDx * xOffset;

				const real& uh = uv[0];
				const real& vh = uv[1];
				const real& wh = t[0];

				// (d/dx) u(x, y)
				// = (d/dx) (u(x, y) / w(x, y)) * w(x, y)
				// = [(d/dx) (u(x, y) / w(x, y))] * w(x, y) + (u(x, y) / w(x, y)) * [(d/dx) w(x, y)]

				// (d/dx) w(x, y)
				// = (d/dx) (1 / w(x, y)) w^2(x, y)
				// = [(d/dx) (1 / w(x, y))] w^2(x, y) + (1 / w(x, y)) (d/dx) w^2(x, y)
				// = [(d/dx) (1 / w(x, y))] w^2(x, y) + (1 / w(x, y)) 2 w(x, y) (d/dx) w(x, y)
				// = [(d/dx) (1 / w(x, y))] w^2(x, y) + 2(d/dx) w(x, y)
				// =>
				// (d/dx) w(x, y)  = -[(d/dx) (1 / w(x, y))] w^2(x, y)

				for (integer x = xBegin;x < xEnd;++x)
				{
					const real w = inverse(wh);

					const real dwDx = -dtDx[0] * w * w;
					const real dwDy = -dtDy[0] * w * w;

					Vector2 dsDx = dUvDx * w + asVector(uv) * dwDx;
					Vector2 dsDy = dUvDy * w + asVector(uv) * dwDy;

					image(x, y) =
						colorMixer(image(x, y),
						textureSampler(Point2(asVector(uv) * w), dsDx, dsDy));

					uv += dUvDx;
					t += dtDx;

					//dsDx += d2sDxDx;
					//dsDy += d2sDyDx;
				}
			}

			xLeft += dxLeftDy;
			uvLeft += dUvLeftDy;
			tLeft += dtLeftDy;

			xRight += dxRightDy;
			uvRight += dUvRightDy;
			tRight += dtRightDy;
		}
	}

	template <typename Type,
		typename Image_View>
	void drawTriangle(
		const Triangle3& triangle,
		const Triangle2& uvTriangle,
		const Texture<Type>& textureSampler,
		const View<2, Type, Image_View>& image)
	{
		drawTriangle(triangle, uvTriangle,
			textureSampler, image, assignColorMixer<Type>());
	}

}

#endif
