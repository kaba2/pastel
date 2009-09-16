// Description: Algorithms for GfxRenderer

#ifndef PASTEL_GFXRENDERERTOOLS_H
#define PASTEL_GFXRENDERERTOOLS_H

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
		GfxRenderer<Type>& renderer,
		const Vector2& point);

	template <typename Type>
	void drawTriangle(
		GfxRenderer<Type>& renderer,
		const Triangle2& triangle);

	template <typename Type>
	void drawTriangle(
		GfxRenderer<Type>& renderer,
		const Triangle2& triangle,
		const Tuple<Type, 3>& colorTriangle);

	template <typename Type>
	void drawTriangle(
		GfxRenderer<Type>& renderer,
		const Triangle2& triangle,
		const Triangle2& textureTriangle);

	template <typename Type>
	void drawSegment(
		GfxRenderer<Type>& renderer,
		const Segment2& segment);

	template <typename Type>
	void drawFatSegment(
		GfxRenderer<Type>& renderer,
		const Segment2& segment,
		real startRadius,
		real endRadius);

	template <typename Type>
	void drawCircle(
		GfxRenderer<Type>& renderer,
		const Sphere2& circle,
		integer segments = 20);

	template <typename Type>
	void drawBox(
		GfxRenderer<Type>& renderer,
		const AlignedBox2& alignedBox);

	template <typename Type>
	void drawBox(
		GfxRenderer<Type>& renderer,
		const AlignedBox2& alignedBox,
		const Tuple<Vector2, 4>& textureQuad);

	template <typename Type>
	void drawBox(
		GfxRenderer<Type>& renderer,
		const Box2& box);

	template <typename Type>
	void drawBox(
		GfxRenderer<Type>& renderer,
		const Box2& box,
		const Tuple<Vector2, 4>& textureQuad);

	template <typename Type>
	void drawArrow(
		GfxRenderer<Type>& renderer,
		const Segment2& segment,
		const real radius);

	template <typename Type>
	void drawSegment(
		GfxRenderer<Type>& renderer,
		const Segment2& segment,
		const std::vector<Vector2>& pen);

	template <typename Type>
	void drawOrientedSegment(
		GfxRenderer<Type>& renderer,
		const Segment2& segment,
		const std::vector<Vector2>& pen);

	template <typename Type>
	void drawConvexPolygon(
		GfxRenderer<Type>& renderer,
		const std::vector<Vector2>& convexPolygon);

}

#include "pastel/gfx/gfxrenderer_tools.hpp"

#endif
