// Description: Algorithms for const GfxRenderer

#ifndef PASTEL_GFXRENDERER_TOOLS_H
#define PASTEL_GFXRENDERER_TOOLS_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/gfxrenderer.h"

#include "pastel/geometry/triangle.h"
#include "pastel/geometry/sphere.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	template <typename Type>
	void drawPoint(
		const GfxRenderer<Type>& renderer,
		const Vector2& point);

	template <typename Type>
	void drawTriangle(
		const GfxRenderer<Type>& renderer,
		const Triangle2& triangle);

	template <typename Type>
	void drawTriangle(
		const GfxRenderer<Type>& renderer,
		const Triangle2& triangle,
		const Tuple<Type, 3>& colorTriangle);

	template <typename Type>
	void drawTriangle(
		const GfxRenderer<Type>& renderer,
		const Triangle2& triangle,
		const Triangle2& textureTriangle);

	template <typename Type>
	void drawSegment(
		const GfxRenderer<Type>& renderer,
		const Segment2& segment);

	template <typename Type>
	void drawFatSegment(
		const GfxRenderer<Type>& renderer,
		const Segment2& segment,
		real startRadius,
		real endRadius);

	template <typename Type>
	void drawCircle(
		const GfxRenderer<Type>& renderer,
		const Sphere2& circle,
		integer segments = 20);

	template <typename Type>
	void drawBox(
		const GfxRenderer<Type>& renderer,
		const AlignedBox2& alignedBox);

	template <typename Type>
	void drawBox(
		const GfxRenderer<Type>& renderer,
		const AlignedBox2& alignedBox,
		const Tuple<Vector2, 4>& textureQuad);

	template <typename Type>
	void drawBox(
		const GfxRenderer<Type>& renderer,
		const Box2& box);

	template <typename Type>
	void drawBox(
		const GfxRenderer<Type>& renderer,
		const Box2& box,
		const Tuple<Vector2, 4>& textureQuad);

	template <typename Type>
	void drawArrow(
		const GfxRenderer<Type>& renderer,
		const Segment2& segment,
		const real radius);

	template <typename Type>
	void drawConvexPolygon(
		const GfxRenderer<Type>& renderer,
		const std::vector<Vector2>& convexPolygon);

}

#include "pastel/gfx/gfxrenderer_tools.hpp"

#endif
