// Description: Algorithms for GfxRenderer
// Documentation: renderer.txt

#ifndef PASTELGFX_GFXRENDERER_TOOLS_H
#define PASTELGFX_GFXRENDERER_TOOLS_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/gfxrenderer.h"

#include "pastel/geometry/triangle.h"
#include "pastel/geometry/sphere.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	//! Draws a one-pixel point.
	template <typename Type>
	void drawPoint(
		const GfxRenderer<Type>& renderer,
		const Vector2& point);

	//! Draws a triangle.
	template <typename Type>
	void drawTriangle(
		const GfxRenderer<Type>& renderer,
		const Triangle2& triangle);

	//! Draws a triangle with linearly interpolated colors.
	template <typename Type>
	void drawTriangle(
		const GfxRenderer<Type>& renderer,
		const Triangle2& triangle,
		const Tuple<Type, 3>& colorTriangle);

	//! Draws a triangle with texturing.
	template <typename Type>
	void drawTriangle(
		const GfxRenderer<Type>& renderer,
		const Triangle2& triangle,
		const Triangle2& textureTriangle);

	//! Draws a line segment.
	template <typename Type>
	void drawSegment(
		const GfxRenderer<Type>& renderer,
		const Segment2& segment);

	//! Draws a fat line segment.
	/*!
	Preconditions:
	startRadius >= 0
	endRadius >= 0

	The width of the line segment is linearly interpolated from
	the start to the end.
	*/
	template <typename Type>
	void drawFatSegment(
		const GfxRenderer<Type>& renderer,
		const Segment2& segment,
		real startRadius,
		real endRadius);

	//! Draws a circle.
	/*!
	The circle is approximated by triangles. The number of used
	segments determines the roundness of the shape.
	*/
	template <typename Type>
	void drawCircle(
		const GfxRenderer<Type>& renderer,
		const Sphere2& circle,
		integer segments = 20);

	//! Draws an axis-aligned box.
	template <typename Type>
	void drawBox(
		const GfxRenderer<Type>& renderer,
		const AlignedBox2& alignedBox);

	//! Draws an axis-aligned box with texturing.
	template <typename Type>
	void drawBox(
		const GfxRenderer<Type>& renderer,
		const AlignedBox2& alignedBox,
		const Tuple<Vector2, 4>& textureQuad);

	//! Draws an oriented box.
	template <typename Type>
	void drawBox(
		const GfxRenderer<Type>& renderer,
		const Box2& box);

	//! Draws an oriented box with texturing.
	template <typename Type>
	void drawBox(
		const GfxRenderer<Type>& renderer,
		const Box2& box,
		const Tuple<Vector2, 4>& textureQuad);

	//! Draws a fat arrow.
	/*!
	Preconditions:
	radius >= 0
	*/
	template <typename Type>
	void drawArrow(
		const GfxRenderer<Type>& renderer,
		const Segment2& segment,
		const real radius);

	//! Draws a convex polygon.
	/*!
	It is essential that the polygon is convex:
	non-convex polygons will not in general be
	drawn correctly.
	*/
	template <typename Type>
	void drawConvexPolygon(
		const GfxRenderer<Type>& renderer,
		const std::vector<Vector2>& convexPolygon);

	//! Sets the view so that a region fills almost all of it.
	template <typename Type>
	void concentrate(
		GfxRenderer<Type>& renderer,
		const AlignedBox2& region);

}

#include "pastel/gfx/gfxrenderer_tools.hpp"

#endif
