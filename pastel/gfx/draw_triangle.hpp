#ifndef PASTELGFX_DRAW_TRIANGLE_HPP
#define PASTELGFX_DRAW_TRIANGLE_HPP

#include "pastel/gfx/draw_triangle.h"
#include "pastel/gfx/draw_horizontal_line.h"
#include "pastel/gfx/assign_colormixer.h"

#include "pastel/geometry/plane.h"
#include "pastel/geometry/predicates.h"

#include "pastel/sys/syscommon.h"

#include <vector>
#include <algorithm>

namespace Pastel
{

	namespace SolidTriangle_
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
				const Vector2& position,
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

			Vector2 position_;
			integer index_;
		};

	}

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawTriangle(
		const Triangle2& triangle,
		const NoDeduction<Type>& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		using Vertex = SolidTriangle_::Vertex;

		std::vector<Vertex> vertex;
		vertex.reserve(3);
		vertex.push_back(Vertex(triangle[0], 0));
		vertex.push_back(Vertex(triangle[1], 1));
		vertex.push_back(Vertex(triangle[2], 2));
		// Sort the triangle vertices lexicographically.
		std::sort(vertex.begin(), vertex.end());

		bool longLeftEdge = side(vertex[1].position_,
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
		const NoDeduction<Type>& color,
		const View<2, Type, Image_View>& image)
	{
		drawTriangle(triangle, color, image, assignColorMixer<Type>());
	}

}

#endif
