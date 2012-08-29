#ifndef PASTELGFX_GFXRENDERERTOOLS_HPP
#define PASTELGFX_GFXRENDERERTOOLS_HPP

#include "pastel/gfx/gfxrenderer_tools.h"

#include "pastel/sys/math_functions.h"

#include "pastel/math/coordinates.h"
#include "pastel/math/affinetransformation_tools.h"

namespace Pastel
{

	template <typename Type>
	void drawPoint(
		const GfxRenderer<Type>& renderer,
		const Vector2& point)
	{
		renderer.drawPoint(point);
	}

	template <typename Type>
	void drawTriangle(
		const GfxRenderer<Type>& renderer,
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
		const GfxRenderer<Type>& renderer,
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
		const GfxRenderer<Type>& renderer,
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
		const GfxRenderer<Type>& renderer,
		const Segment2& segment)
	{
		renderer.drawSegment(segment);
	}

	template <typename Type>
	void drawFatSegment(
		const GfxRenderer<Type>& renderer,
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

		const Vector2 startLeft = segment.start() + normal * startRadius;
		const Vector2 startRight = segment.start() - normal * startRadius;
		const Vector2 endLeft = segment.end() + normal * endRadius;
		const Vector2 endRight = segment.end() - normal * endRadius;

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
		const GfxRenderer<Type>& renderer,
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
		const GfxRenderer<Type>& renderer,
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
		const GfxRenderer<Type>& renderer,
		const AlignedBox2& alignedBox,
		const Tuple<Vector2, 4>& textureQuad)
	{
		const Box2 box(
			linear(alignedBox.min(), alignedBox.max(), 0.5),
			(alignedBox.max() - alignedBox.min()) * 0.5,
			Matrix2(Vector2(1, 0), Vector2(0, 1)));

		drawBox(renderer, box, textureQuad);
	}

	template <typename Type>
	void drawBox(
		const GfxRenderer<Type>& renderer,
		const Box2& box)
	{
		const Vector2& center = box.position();
		const Vector2 x = box.rotation()[0] * box.width()[0];
		const Vector2 y = box.rotation()[1] * box.width()[1];

		const Vector2 leftBottom = center - x - y;
		const Vector2 rightBottom = center + x - y;
		const Vector2 rightTop = center + x + y;
		const Vector2 leftTop = center - x + y;

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
		const GfxRenderer<Type>& renderer,
		const Box2& box,
		const Tuple<Vector2, 4>& textureQuad)
	{
		const Vector2& center = box.position();
		const Vector2 x = box.rotation()[0] * box.width()[0];
		const Vector2 y = box.rotation()[1] * box.width()[1];

		const Vector2 leftBottom = center - x - y;
		const Vector2 rightBottom = center + x - y;
		const Vector2 rightTop = center + x + y;
		const Vector2 leftTop = center - x + y;

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
		const GfxRenderer<Type>& renderer,
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

		const Vector2 endLeft = segment.end() + normal * radius - tangent * radius * 2;
		const Vector2 endRight = segment.end() - normal * radius - tangent * radius * 2;

		drawFatSegment(renderer, Segment2(segment.start(), segment.end() - tangent * radius * 2),
			radius * 0.15, radius * 0.15);
		drawTriangle(renderer, Triangle2(segment.end(), endLeft, endRight));
	}

	template <typename Type>
	void drawConvexPolygon(
		const GfxRenderer<Type>& renderer,
		const std::vector<Vector2>& convexPolygon)
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

	template <typename Type>
	void concentrate(
		GfxRenderer<Type>& renderer,
		const AlignedBox2& region)
	{
		AlignedBox2 viewWindow = region;

		const Vector2 cameraCenter = 
			midpoint(viewWindow.min(), viewWindow.max());

		viewWindow -= cameraCenter;

		viewWindow.min() -= viewWindow.extent() * 0.05;
		viewWindow.max() += viewWindow.extent() * 0.05;
		const Vector2 viewExtent = viewWindow.extent();

		const real aspectRatio = (real)4 / 3;
		if (viewExtent.x() < aspectRatio * viewExtent.y())
		{
			const real xExtentDelta = 
				aspectRatio * viewExtent.y() - viewExtent.x();

			viewWindow.min().x() -= xExtentDelta / 2;
			viewWindow.max().x() += xExtentDelta / 2;
		}
		else
		{
			const real yExtentDelta = 
				viewExtent.x() - aspectRatio * viewExtent.y();

			viewWindow.min().y() -= yExtentDelta / 2;
			viewWindow.max().y() += yExtentDelta / 2;
		}

		renderer.setViewTransformation(
			translation2<real>(cameraCenter));
		renderer.setViewWindow(viewWindow);
	}

}

#endif
