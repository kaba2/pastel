#ifndef PASTEL_GFXRENDERERTOOLS_HPP
#define PASTEL_GFXRENDERERTOOLS_HPP

#include "pastel/gfx/gfxrenderer_tools.h"

#include "pastel/geometry/antipodal.h"
#include "pastel/geometry/sweep.h"

#include "pastel/sys/math_functions.h"
#include "pastel/math/coordinates.h"

namespace Pastel
{

	template <typename Type>
	void drawPoint(
		GfxRenderer<Type>& renderer,
		const Point2& point)
	{
		renderer.drawPoint(point);
	}

	template <typename Type>
	void drawTriangle(
		GfxRenderer<Type>& renderer,
		const Triangle2& triangle)
	{
		if (renderer.filled())
		{
			renderer.drawTriangle(triangle);
		}
		else
		{
			renderer.drawSegment(Segment2(triangle[0], triangle[1]));
			renderer.drawSegment(Segment2(triangle[1], triangle[2]));
			renderer.drawSegment(Segment2(triangle[2], triangle[0]));
		}
	}

	template <typename Type>
	void drawTriangle(
		GfxRenderer<Type>& renderer,
		const Triangle2& triangle,
		const Tuple<Type, 3>& colorTriangle)
	{
		if (renderer.filled())
		{
			renderer.drawTriangle(triangle, colorTriangle);
		}
		else
		{
			renderer.drawSegment(Segment2(triangle[0], triangle[1]));
			renderer.drawSegment(Segment2(triangle[1], triangle[2]));
			renderer.drawSegment(Segment2(triangle[2], triangle[0]));
		}
	}

	template <typename Type>
	void drawTriangle(
		GfxRenderer<Type>& renderer,
		const Triangle2& triangle,
		const Triangle2& textureTriangle)
	{
		if (renderer.filled())
		{
			renderer.drawTriangle(triangle, textureTriangle);
		}
		else
		{
			renderer.drawSegment(Segment2(triangle[0], triangle[1]));
			renderer.drawSegment(Segment2(triangle[1], triangle[2]));
			renderer.drawSegment(Segment2(triangle[2], triangle[0]));
		}
	}

	template <typename Type>
	void drawSegment(
		GfxRenderer<Type>& renderer,
		const Segment2& segment)
	{
		renderer.drawSegment(segment);
	}

	template <typename Type>
	void drawFatSegment(
		GfxRenderer<Type>& renderer,
		const Segment2& segment,
		real startRadius,
		real endRadius)
	{
		ENSURE_OP(startRadius, >=, 0);
		ENSURE_OP(endRadius, >=, 0);

		const Vector2 delta = segment.end() - segment.start();
		const real normDelta = norm(delta);

		if (normDelta == 0)
		{
			return;
		}

		const Vector2 normal = cross(delta) / normDelta;

		const Point2 startLeft = segment.start() + normal * startRadius;
		const Point2 startRight = segment.start() - normal * startRadius;
		const Point2 endLeft = segment.end() + normal * endRadius;
		const Point2 endRight = segment.end() - normal * endRadius;

		if (renderer.filled())
		{
			renderer.drawTriangle(Triangle2(startLeft, startRight, endLeft));
			renderer.drawTriangle(Triangle2(startRight, endRight, endLeft));
		}
		else
		{
			renderer.drawSegment(Segment2(startLeft, startRight));
			renderer.drawSegment(Segment2(startRight, endRight));
			renderer.drawSegment(Segment2(endRight, endLeft));
			renderer.drawSegment(Segment2(endLeft, startLeft));
		}
	}

	template <typename Type>
	void drawCircle(
		GfxRenderer<Type>& renderer,
		const Sphere2& circle,
		integer segments)
	{
		ENSURE_OP(segments, >=, 0);

		if (segments <= 1)
		{
			return;
		}

		const real angleAdd = (2 * constantPi<real>()) / segments;

		if (!renderer.filled())
		{
			real angle = 0;
			real nextAngle = angleAdd;
			for (integer i = 0;i < segments;++i)
			{
				drawSegment(
					renderer,
					Segment2(
					circle.position() +
					sphericalToCartesian(Vector2(circle.radius(), angle)),
					circle.position() +
					sphericalToCartesian(Vector2(circle.radius(), nextAngle))));

				angle = nextAngle;
				nextAngle += angleAdd;
			}
		}
		else
		{
			real angle = 0;
			real nextAngle = angleAdd;
			for (integer i = 0;i < segments;++i)
			{
				drawTriangle(
					renderer,
					Triangle2(
					circle.position(),
					circle.position() +
					sphericalToCartesian(Vector2(circle.radius(), angle)),
					circle.position() +
					sphericalToCartesian(Vector2(circle.radius(), nextAngle))));

				angle = nextAngle;
				nextAngle += angleAdd;
			}
		}
	}

	template <typename Type>
	void drawBox(
		GfxRenderer<Type>& renderer,
		const AlignedBox2& alignedBox)
	{
		const Box2 box(
			linear(alignedBox.min(), alignedBox.max(), 0.5),
			(alignedBox.max() - alignedBox.min()) * 0.5,
			Matrix2(Vector2(1, 0), Vector2(0, 1)));

		drawBox(renderer, box);
	}

	template <typename Type>
	void drawBox(
		GfxRenderer<Type>& renderer,
		const AlignedBox2& alignedBox,
		const Tuple<Point2, 4>& textureQuad)
	{
		const Box2 box(
			linear(alignedBox.min(), alignedBox.max(), 0.5),
			(alignedBox.max() - alignedBox.min()) * 0.5,
			Matrix2(Vector2(1, 0), Vector2(0, 1)));

		drawBox(renderer, box, textureQuad);
	}

	template <typename Type>
	void drawBox(
		GfxRenderer<Type>& renderer,
		const Box2& box)
	{
		const Point2& center = box.position();
		const Vector2 x = box.rotation()[0] * box.width()[0];
		const Vector2 y = box.rotation()[1] * box.width()[1];

		const Point2 leftBottom = center - x - y;
		const Point2 rightBottom = center + x - y;
		const Point2 rightTop = center + x + y;
		const Point2 leftTop = center - x + y;

		if (!renderer.filled())
		{
			drawSegment(
				renderer,
				Segment2(leftBottom, rightBottom));
			drawSegment(
				renderer,
				Segment2(rightBottom, rightTop));
			drawSegment(
				renderer,
				Segment2(rightTop, leftTop));
			drawSegment(
				renderer,
				Segment2(leftTop, leftBottom));
		}
		else
		{
			drawTriangle(
				renderer,
				Triangle2(leftBottom, rightBottom, leftTop));
			drawTriangle(
				renderer,
				Triangle2(rightBottom, rightTop, leftTop));
		}
	}

	template <typename Type>
	void drawBox(
		GfxRenderer<Type>& renderer,
		const Box2& box,
		const Tuple<Point2, 4>& textureQuad)
	{
		const Point2& center = box.position();
		const Vector2 x = box.rotation()[0] * box.width()[0];
		const Vector2 y = box.rotation()[1] * box.width()[1];

		const Point2 leftBottom = center - x - y;
		const Point2 rightBottom = center + x - y;
		const Point2 rightTop = center + x + y;
		const Point2 leftTop = center - x + y;

		if (!renderer.filled())
		{
			drawSegment(
				renderer,
				Segment2(leftBottom, rightBottom));
			drawSegment(
				renderer,
				Segment2(rightBottom, rightTop));
			drawSegment(
				renderer,
				Segment2(rightTop, leftTop));
			drawSegment(
				renderer,
				Segment2(leftTop, leftBottom));
		}
		else
		{
			drawTriangle(
				renderer,
				Triangle2(leftBottom, rightBottom, leftTop),
				makeTuple(textureQuad[0], textureQuad[1], textureQuad[3]));
			drawTriangle(
				renderer,
				Triangle2(rightBottom, rightTop, leftTop),
				makeTuple(textureQuad[1], textureQuad[2], textureQuad[3]));
		}
	}

	template <typename Type>
	void drawArrow(
		GfxRenderer<Type>& renderer,
		const Segment2& segment,
		const real radius)
	{
		ENSURE_OP(radius, >=, 0);

		const Vector2 delta = segment.end() - segment.start();
		const real normDelta = norm(delta);

		if (normDelta < 2 * radius)
		{
			return;
		}

		const Vector2 tangent = delta / normDelta;
		const Vector2 normal = cross(tangent);

		const Point2 endLeft = segment.end() + normal * radius - tangent * radius * 2;
		const Point2 endRight = segment.end() - normal * radius - tangent * radius * 2;

		drawFatSegment(renderer, Segment2(segment.start(), segment.end() - tangent * radius * 2),
			radius * 0.15, radius * 0.15);
		drawTriangle(renderer, Triangle2(segment.end(), endLeft, endRight));
	}

	template <typename Type>
	void drawSegment(
		GfxRenderer<Type>& renderer,
		const Segment2& segment,
		const std::vector<Point2>& pen)
	{
		std::vector<Point2> sweepPolygon;
		sweep(pen, segment, sweepPolygon);

		drawConvexPolygon(renderer, sweepPolygon);
	}

	template <typename Type>
	void drawOrientedSegment(
		GfxRenderer<Type>& renderer,
		const Segment2& segment,
		const std::vector<Point2>& pen)
	{
		std::vector<Point2> sweepPolygon;
		orientedSweep(pen, segment, sweepPolygon);

		drawConvexPolygon(renderer, sweepPolygon);
	}

	template <typename Type>
	void drawConvexPolygon(
		GfxRenderer<Type>& renderer,
		const std::vector<Point2>& convexPolygon)
	{
		const integer points = convexPolygon.size();

		if (!renderer.filled())
		{
			for (integer i = 0;i < points - 1;++i)
			{
				drawSegment(renderer, Segment2(convexPolygon[i], convexPolygon[i + 1]));
			}
			drawSegment(renderer, Segment2(convexPolygon[points - 1], convexPolygon[0]));
		}
		else
		{
			for (integer i = 1;i < points - 1;++i)
			{
				drawTriangle(renderer, Triangle2(convexPolygon[0], convexPolygon[i], convexPolygon[i + 1]));
			}
		}
	}

}

#endif
