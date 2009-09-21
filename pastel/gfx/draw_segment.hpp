#ifndef PASTEL_DRAW_SEGMENT_HPP
#define PASTEL_DRAW_SEGMENT_HPP

#include "pastel/gfx/draw_segment.h"
#include "pastel/gfx/draw_pixel.h"

#include "pastel/geometry/intersect_alignedbox_segment.h"

namespace Pastel
{

	template <typename Type, typename Image_View>
	void drawSegmentOld(
		const Segment2& segment,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image)
	{
		// The Bresenham line algorithm for drawing line segments.
		//
		// This is a generalized version for floating point end points,
		// derived here.
		//
		// Our task is to the generate integer sequences
		// x(i) and y(i) such that the set {(x(i), y(i))}
		// forms a one-pixel wide line segment from
		// (xStart, yStart) to (xEnd, yEnd).
		//
		// We take the sampling point of a pixel (x, y) to
		// be at (x + u, y + v). For example if u, v = 0.5,
		// then the sampling point of the pixel (0, 0) is at
		// (0.5, 0.5).
		//
		// Let
		// dx = xEnd - xStart
		// dy = yEnd - yStart
		//
		// The algorithm separates into cases based on which
		// octant the end point is with respect to the start point.
		// The octants are numbered as:
		//
		// \2|1/
		// 3\|/0
		// --+--
		// 4/|\7
		// /5|6\
		//
		// Assume |dx| >= |dy|. This takes care
		// of octants 0, 3, 4 and 7. We will deal
		// with the other octants later.
		//
		// We start tracing from the pixel (x(0), y(0))
		// that has the sampling point closest to (xStart, yStart).
		//
		// From here, we perform either of the
		// two steps:
		// 1) x(i + 1) = x(i) + tx, y(i + 1) = y(i) + ty1
		// 2) x(i + 1) = x(i) + tx, y(i + 1) = y(i) + ty2
		// where
		// (tx, ty1, ty2) is one of
		// {(1, 0, 1), (1, 0, -1), (-1, 0, 1), (-1, 0, -1)}.
		//
		// We choose the one that will keep us closer to the line.
		// This simple procedure is repeated to trace the whole line
		// segment.
		//
		// The distance of a pixel to the line is measured as the
		// distance of its sampling point to the line. Thus the
		// comparison to select between the two cases is:
		//
		// distance((x(i) + u + tx, y(i) + v + ty1), line) <=
		// distance((x(i) + u + tx, y(i) + v + ty2), line)
		//
		// If this comparison yields true, the option 1 is chosen.
		// Otherwise option 2 is chosen. However, this is inefficient.
		//
		// Let
		// a(i) = x(i) + u
		// b(i) = yStart + ((x(i) + u) - xStart) * (dy / dx)
		//
		// Then (a(i), b(i)) traces out points from the line
		// with a(i) horizontally at the sampling points.
		//
		// Because of similar triangles, we can equivalently compare the
		// distances of the sampling points to the line along the y-axis:
		//
		// |(y(i) + v + ty1) - b(i + 1)| <= |b(i + 1) - (y(i) + v + ty2)|
		//
		// This is much faster, but the absolute values
		// still block the way to an efficient implementation.
		//
		// Fortunately, we see that there is still one more equivalent
		// comparison. Rather than comparing the distances of the two candidate points
		// to the line segment, we can compare the position of their
		// midpoint to the line segment (along y-axis):
		//
		// ((y(i) + v + ty1) + (y(i) + v + ty2)) / 2 <= b(i + 1)
		// <=>
		// (y(i) + v + ty1) + (y(i) + v + ty2) <= 2 b(i + 1)
		// <=>
		// (y(i) + v + ty1) - b(i + 1) <= b(i + 1) - (y(i) + v + ty2)
		//
		// Thus, the absolute values can be removed!
		//
		// Rather than comparing directly (left <= right), we compare the sign
		// of the difference (left - right <= 0):
		//
		// s'(i)
		// = (y(i) + v + ty1) - b(i + 1)) - (b(i + 1) - (y(i) + v + ty2))
		// = 2(y(i) + v) - 2b(i + 1) + (ty1 + ty2)
		// = 2(y(i) + v) - 2(yStart + (x(i + 1) + v - xStart) * (dy / dx)) + (ty1 + ty2)
		// = 2(y(i) + v) - 2(yStart + (x(i) + tx + v - xStart) * (dy / dx)) + (ty1 + ty2)
		// = 2(y(i) + v) - 2b(i) - 2 tx (dy / dx) + (ty1 + ty2)
		//
		// If s'(i) <= 0, then we choose the option that increases s'(i).
		// Otherwise we choose the option that decreases s'(i).
		//
		// Because dx is positive, and thus does not change the sign
		// of s', we can as well multiply s' by dx to avoid division.
		//
		// s(i)
		// = s'(i) * dx
		// = 2(y(i) + v)dx - 2b(i)dx - 2 tx dy + (ty1 + ty2)dx
		//
		// We now notice:
		//
		// If option 1 is chosen, then
		// s(i + 1)
		// = 2(y(i + 1) + v)dx - 2b(i + 1)dx - 2 tx dy + (ty1 + ty2)dx
		// = 2(y(i) + v)dx + 2 ty1 dx - 2b(i)dx - 2 tx dy - 2 tx dy + (ty1 + ty2)dx
		// = s(i) + 2 ty1 dx - 2 tx dy
		//
		// Similarly, if option 2 is chosen, then
		// s(i + 1)
		// = s(i) + 2 ty2 dx - 2 tx dy
		//
		// Thus the values for s(i) can be computed
		// incrementally. However, s(0) must be computed directly:
		// s(0)
		// = 2(y(0) + v)dx - 2b(0)dx - 2 tx dy + (ty1 + ty2)dx
		// = 2(y(0) + v)dx - 2(yStart * dx + ((x(0) + u) - xStart) * dy) - 2 tx dy + (ty1 + ty2)dx
		// = 2((y(0) + v) - yStart)dx - 2((x(0) + u) - xStart)dy - 2 tx dy + (ty1 + ty2) dx
		//
		// Notice that if xStart = x(0) + u and yStart = y(0) + v (the
		// start point is at a sample point), then
		// s(0) = -2 tx dy + (ty1 + ty2) dx
		// If in addition (xEnd, yEnd) is at a sample point,
		// then the whole algorithm runs on integer arithmetic.
		//
		// The octants 1, 2, 5, and 6 with |dx| < |dy| can be derived
		// by simply exchanging x and y in all formulas.
		// This has the effect of reflecting the plane
		// w.r.t the line (1, 1).
		// Thus the sAdd1, sAdd2 and s(0) can be obtained from
		// the following reflection pairs by exchanging x and y:
		//
		// Octant 1 <-> Octant 0
		// Octant 2 <-> Octant 7
		// Octant 5 <-> Octant 4
		// Octant 6 <-> Octant 3
		//
		// Summary
		// -------
		//
		// sNormal =
		// 2((y(0) + v) - yStart)dx - 2((x(0) + u) - xStart)dy - 2 tx dy + (ty1 + ty2) dx
		// sReflected =
		// 2((x(0) + u) - xStart)dy - 2((y(0) + v) - yStart)dx - 2 ty dx + (tx1 + tx2) dy
		//
		// Octant 0:
		// (1, 0) => sAdd1 = -2 dy
		// (1, 1) => sAdd2 = -2 dy + 2 dx
		// s(0) = sNormal + -2 dy + dx
		//
		// Octant 1 (reflected octant 0):
		// (0, 1) => sAdd1 = -2 dx
		// (1, 1) => sAdd2 = -2 dx + 2 dy
		// s(0) = sReflected + (-2 dx + dy)
		//
		// Octant 3:
		// (-1, 0) => sAdd1 = 2 dy
		// (-1, 1) => sAdd2 = 2 dy + 2 dx
		// s(0) = sNormal + 2 dy + dx
		//
		// Octant 6 (reflected octant 3):
		// (1, -1) => sAdd1 = 2 dx + 2 dy
		// (0, -1) => sAdd2 = 2 dx
		// s(0) = sReflected + (2 dx + dy)
		//
		// Octant 4:
		// (-1,  0) => sAdd1 = 2 dy
		// (-1, -1) => sAdd2 = 2 dy - 2 dx
		// s(0) = sNormal + 2 dy - dx
		//
		// Octant 5 (reflected octant 4):
		// ( 0, -1) => sAdd1 = 2 dx
		// (-1, -1) => sAdd2 = 2 dx - 2 dy
		// s(0) = sReflected + (2 dx - dy)
		//
		// Octant 7:
		// (1,  0) => sAdd1 = -2 dy
		// (1, -1) => sAdd2 = -2 dy - 2 dx
		// s(0) = sNormal + (-2 dy - dx)
		//
		// Octant 2 (reflected octant 7):
		// (-1, 1) => sAdd1 = -2 dx - 2 dy
		// ( 0, 1) => sAdd2 = -2 dx
		// s(0) = sReflected + (-2 dx - dy)

		Segment2 clippedSegment;
		if (!intersect(AlignedBox2(0, 0, image.width(), image.height()), segment,
			clippedSegment))
		{
			return;
		}

		real sAdd1 = 0;
		real sAdd2 = 0;

		const real xStart = clippedSegment.start().x();
		const real yStart = clippedSegment.start().y();
		const real xEnd = clippedSegment.end().x();
		const real yEnd = clippedSegment.end().y();

		const real dx = xEnd - xStart;
		const real dy = yEnd - yStart;

		integer x = std::floor(xStart);
		integer y = std::floor(yStart);

		const integer steps = std::max(
			mabs(std::floor(xEnd) - x),
			mabs(std::floor(yEnd) - y)) + 1;

		if (mabs(dx) >= mabs(dy))
		{
			real s =
				2 * (((real)y + (real)0.5) - yStart) * dx -
				2 * (((real)x + (real)0.5) - xStart) * dy;

			if (dx >= 0)
			{
				if (dy >= 0)
				{
					// Octant 0

					sAdd1 = 2 * (-dy);
					sAdd2 = 2 * (dx - dy);

					s += -2 * dy + dx;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						++x;

						if (s >= 0)
						{
							s += sAdd1;
						}
						else
						{
							++y;
							s += sAdd2;
						}
					}
				}
				else
				{
					// Octant 7

					sAdd1 = 2 * (-dx - dy);
					sAdd2 = 2 * (-dy);

					s += -2 * dy - dx;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						++x;

						if (s >= 0)
						{
							--y;
							s += sAdd1;
						}
						else
						{
							s += sAdd2;
						}
					}
				}
			}
			else
			{
				if (dy >= 0)
				{
					// Octant 3

					sAdd1 = 2 * (dx + dy);
					sAdd2 = 2 * dy;

					s += 2 * dy + dx;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						--x;

						if (s >= 0)
						{
							++y;
							s += sAdd1;
						}
						else
						{
							s += sAdd2;
						}
					}
				}
				else
				{
					// Octant 4

					sAdd1 = 2 * dy;
					sAdd2 = 2 * (-dx + dy);

					s += 2 * dy - dx;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						--x;

						if (s >= 0)
						{
							s += sAdd1;
						}
						else
						{
							--y;
							s += sAdd2;
						}
					}
				}
			}
		}
		else
		{
			real s =
				2 * (((real)x + (real)0.5) - xStart) * dy -
				2 * (((real)y + (real)0.5) - yStart) * dx;

			if (dx >= 0)
			{
				if (dy >= 0)
				{
					// Octant 1

					sAdd1 = -2 * dx;
					sAdd2 = 2 * (-dx + dy);

					s += -2 * dx + dy;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						++y;

						if (s >= 0)
						{
							s += sAdd1;
						}
						else
						{
							++x;
							s += sAdd2;
						}
					}
				}
				else
				{
					// Octant 6

					sAdd1 = 2 * (dx + dy);
					sAdd2 = 2 * dx;

					s += 2 * dx + dy;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						--y;

						if (s >= 0)
						{
							++x;
							s += sAdd1;
						}
						else
						{
							s += sAdd2;
						}
					}
				}
			}
			else
			{
				if (dy >= 0)
				{
					// Octant 2

					sAdd1 = -2 * (dx + dy);
					sAdd2 = -2 * dx;

					s += -2 * dx - dy;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						++y;

						if (s >= 0)
						{
							--x;
							s += sAdd1;
						}
						else
						{
							s += sAdd2;
						}
					}
				}
				else
				{
					// Octant 5

					sAdd1 = 2 * dx;
					sAdd2 = 2 * (dx - dy);

					s += 2 * dx - dy;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						--y;

						if (s >= 0)
						{
							s += sAdd1;
						}
						else
						{
							--x;
							s += sAdd2;
						}
					}
				}
			}
		}
	}

	template <typename Type, typename Image_View>
	void drawSegment(
		const Segment2& segment,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image)
	{
		// See the implementation documentation for drawSegmentOld.

		Segment2 clippedSegment;
		if (!intersect(AlignedBox2(0, 0, image.width(), image.height()), segment,
			clippedSegment))
		{
			return;
		}

		enum
		{
			FixedPoint = 16
		};

		const integer factor = 1 << FixedPoint;

		real sAdd1 = 0;
		real sAdd2 = 0;

		const real xStart = clippedSegment.start().x();
		const real yStart = clippedSegment.start().y();
		const real xEnd = clippedSegment.end().x();
		const real yEnd = clippedSegment.end().y();

		const real dx = xEnd - xStart;
		const real dy = yEnd - yStart;

		integer x = std::floor(xStart);
		integer y = std::floor(yStart);

		const integer steps = std::max(
			mabs(std::floor(xEnd) - x),
			mabs(std::floor(yEnd) - y)) + 1;

		if (mabs(dx) >= mabs(dy))
		{
			real s =
				2 * (((real)y + (real)0.5) - yStart) * dx -
				2 * (((real)x + (real)0.5) - xStart) * dy;

			if (dx >= 0)
			{
				if (dy >= 0)
				{
					// Octant 0

					sAdd1 = 2 * (-dy);
					sAdd2 = 2 * (dx - dy);

					s += -2 * dy + dx;

					integer sFixed = s * factor;
					const integer sFixedAdd1 = sAdd1 * factor;
					const integer sFixedAdd2 = sAdd2 * factor;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						++x;

						if (sFixed >= 0)
						{
							sFixed += sFixedAdd1;
						}
						else
						{
							++y;
							sFixed += sFixedAdd2;
						}
					}
				}
				else
				{
					// Octant 7

					sAdd1 = 2 * (-dx - dy);
					sAdd2 = 2 * (-dy);

					s += -2 * dy - dx;

					integer sFixed = s * factor;
					const integer sFixedAdd1 = sAdd1 * factor;
					const integer sFixedAdd2 = sAdd2 * factor;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						++x;

						if (sFixed >= 0)
						{
							--y;
							sFixed += sFixedAdd1;
						}
						else
						{
							sFixed += sFixedAdd2;
						}
					}
				}
			}
			else
			{
				if (dy >= 0)
				{
					// Octant 3

					sAdd1 = 2 * (dx + dy);
					sAdd2 = 2 * dy;

					s += 2 * dy + dx;

					integer sFixed = s * factor;
					const integer sFixedAdd1 = sAdd1 * factor;
					const integer sFixedAdd2 = sAdd2 * factor;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						--x;

						if (sFixed >= 0)
						{
							++y;
							sFixed += sFixedAdd1;
						}
						else
						{
							sFixed += sFixedAdd2;
						}
					}
				}
				else
				{
					// Octant 4

					sAdd1 = 2 * dy;
					sAdd2 = 2 * (-dx + dy);

					s += 2 * dy - dx;

					integer sFixed = s * factor;
					const integer sFixedAdd1 = sAdd1 * factor;
					const integer sFixedAdd2 = sAdd2 * factor;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						--x;

						if (sFixed >= 0)
						{
							sFixed += sFixedAdd1;
						}
						else
						{
							--y;
							sFixed += sFixedAdd2;
						}
					}
				}
			}
		}
		else
		{
			real s =
				2 * (((real)x + (real)0.5) - xStart) * dy -
				2 * (((real)y + (real)0.5) - yStart) * dx;

			if (dx >= 0)
			{
				if (dy >= 0)
				{
					// Octant 1

					sAdd1 = -2 * dx;
					sAdd2 = 2 * (-dx + dy);

					s += -2 * dx + dy;

					integer sFixed = s * factor;
					const integer sFixedAdd1 = sAdd1 * factor;
					const integer sFixedAdd2 = sAdd2 * factor;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						++y;

						if (sFixed >= 0)
						{
							sFixed += sFixedAdd1;
						}
						else
						{
							++x;
							sFixed += sFixedAdd2;
						}
					}
				}
				else
				{
					// Octant 6

					sAdd1 = 2 * (dx + dy);
					sAdd2 = 2 * dx;

					s += 2 * dx + dy;

					integer sFixed = s * factor;
					const integer sFixedAdd1 = sAdd1 * factor;
					const integer sFixedAdd2 = sAdd2 * factor;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						--y;

						if (sFixed >= 0)
						{
							++x;
							sFixed += sFixedAdd1;
						}
						else
						{
							sFixed += sFixedAdd2;
						}
					}
				}
			}
			else
			{
				if (dy >= 0)
				{
					// Octant 2

					sAdd1 = -2 * (dx + dy);
					sAdd2 = -2 * dx;

					s += -2 * dx - dy;

					integer sFixed = s * factor;
					const integer sFixedAdd1 = sAdd1 * factor;
					const integer sFixedAdd2 = sAdd2 * factor;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						++y;

						if (sFixed >= 0)
						{
							--x;
							sFixed += sFixedAdd1;
						}
						else
						{
							sFixed += sFixedAdd2;
						}
					}
				}
				else
				{
					// Octant 5

					sAdd1 = 2 * dx;
					sAdd2 = 2 * (dx - dy);

					s += 2 * dx - dy;

					integer sFixed = s * factor;
					const integer sFixedAdd1 = sAdd1 * factor;
					const integer sFixedAdd2 = sAdd2 * factor;

					for (integer i = 0;i < steps;++i)
					{
						drawPixel(Vector2i(x, y), color, image);
						--y;

						if (sFixed >= 0)
						{
							sFixed += sFixedAdd1;
						}
						else
						{
							--x;
							sFixed += sFixedAdd2;
						}
					}
				}
			}
		}
	}

}

#endif
