#ifndef PASTEL_DRAWING_HPP
#define PASTEL_DRAWING_HPP

#include "pastel/gfx/drawing.h"
#include "pastel/gfx/assigncolormixer.h"

#include "pastel/sys/linear.h"

#include "pastel/geometry/predicates.h"
#include "pastel/geometry/intersect_alignedbox_segment.h"
#include "pastel/geometry/intersect_alignedbox_alignedbox.h"

#include "pastel/sys/mathfunctions.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/smallset.h"
#include "pastel/sys/array.h"
#include "pastel/sys/view_visit.h"
#include "pastel/sys/subview.h"
#include "pastel/sys/rectangle_tools.h"
#include "pastel/sys/point_tools.h"
#include "pastel/sys/vector_tools.h"

#include <algorithm>

namespace Pastel
{

	template <int N, typename Type, typename Image_View, typename ColorMixer>
	void drawPixel(
		const PASTEL_NO_DEDUCTION((Point<N, integer>))& position,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<N, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		if (anyLess(position, 0) ||
			anyGreaterEqual(asVector(position), image.extent()))
		{
			return;
		}

		image(position) = colorMixer(image(position), color);
	}

	template <int N, typename Type, typename Image_View>
	void drawPixel(
		const PASTEL_NO_DEDUCTION((Point<N, integer>))& position,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<N, Type, Image_View>& image)
	{
		drawPixel(position, color, image, AssignColorMixer<Type>());
	}

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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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
						drawPixel(IPoint2(x, y), color, image);
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

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawHorizontalLine(
		real x, integer y, real x2,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		// Cull by y.

		if (y < 0 || y >= image.height())
		{
			return;
		}

		// Convert to integer coordinates.

		integer xMin = toPixelSpanPoint(x);
		integer xMax = toPixelSpanPoint(x2);

		// Cull by x.

		if (xMin >= image.width() || xMax <= 0)
		{
			return;
		}

		// Clip.

		if (xMin < 0)
		{
			xMin = 0;
		}

		if (xMax > image.width())
		{
			xMax = image.width();
		}

		typedef typename Image_View::Cursor Cursor;

		Cursor cursor = image.cursor(xMin, y);

		// Draw.

		for (integer x = xMin;x < xMax;++x)
		{
			*cursor = colorMixer(*cursor, color);
			cursor.xIncrement();
		}
	}

	template <typename Type, typename Image_View>
	void drawHorizontalLine(
		real x, integer y, real x2,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image)
	{
		drawHorizontalLine(
			x, y, x2, color, image, assignColorMixer<Type>());
	}

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawVerticalLine(
		real x, integer y, real y2,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		// Cull by x.

		if (x < 0 || x >= image.width())
		{
			return;
		}

		// Convert to integer coordinates.

		integer yMin = toPixelSpanPoint(y);
		integer yMax = toPixelSpanPoint(y2);

		// Cull by x.

		if (yMin >= image.height() || yMax <= 0)
		{
			return;
		}

		// Clip.

		if (yMin < 0)
		{
			yMin = 0;
		}

		if (yMax > image.height())
		{
			yMax = image.height();
		}

		typedef typename Image_View::Cursor Cursor;

		Cursor cursor = image.cursor(x, yMin);

		// Draw.

		for (integer y = yMin;y < yMax;++y)
		{
			*cursor = colorMixer(*cursor, color);
			cursor.yIncrement();
		}
	}

	template <typename Type, typename Image_View>
	void drawVerticalLine(
		real x, integer y, real y2,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image)
	{
		drawVerticalLine(
			x, y, y2, color, image, 
			assignColorMixer<Type>());
	}

	template <typename Type, typename Image_View>
	void drawBox(
		const AlignedBox2& box,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image)
	{
		Pastel::drawBox(box, color, image, assignColorMixer<Type>());
	}

	template <typename Type, typename Image_View, typename ColorMixer>
	void drawBox(
		const AlignedBox2& box,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		const Rectangle2 discreteBox(
			toPixelSpanPoint(box.min()), 
			toPixelSpanPoint(box.max()));

		Rectangle2 clippedBox;
		if (!intersect(discreteBox, 
			Rectangle2(IPoint2(0), asPoint(image.extent())), 
			clippedBox))
		{
			return;
		}

		typedef typename Image_View::Cursor Cursor;

		Cursor yCursor = image.cursor(clippedBox.min());

		const integer width = clippedBox.extent().x();
		const integer height = clippedBox.extent().y();

		for (integer y = 0;y < height;++y)
		{
			Cursor xyCursor = yCursor;

			for (integer x = 0;x < width;++x)
			{
				*xyCursor = colorMixer(*xyCursor, color);
				xyCursor.xIncrement();
			}

			yCursor.yIncrement();
		}
	}

	template <typename Type, typename Image_View>
	void floodFill(
		integer xStart, integer yStart,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image)
	{
		const integer width = image.width();
		const integer height = image.height();

		if (xStart < 0 || xStart >= width ||
			yStart < 0 || yStart >= height)
		{
			// Seed point out of range.
			return;
		}

		typedef typename Image_View::Cursor Cursor;

		const Type source = *image.cursor(xStart, yStart);

		if (source == color)
		{
			// No need to do anything.
			return;
		}

		std::vector<Integer2> stack;
		stack.push_back(Integer2(xStart, yStart));

		while (!stack.empty())
		{
			const Integer2 line = stack.back();
			stack.pop_back();

			// Find the extents of this scanline
			// and draw it.

			integer xLeft = line[0];
			integer xRight = line[0];
			const integer y = line[1];

			Cursor yCursor = image.cursor(xLeft, y);

			if (*yCursor != source)
			{
				continue;
			}

			{
				Cursor leftCursor = yCursor;
				leftCursor.xDecrement();

				while(xLeft > 0 && *leftCursor == source)
				{
					*leftCursor = color;

					--xLeft;
					leftCursor.xDecrement();
				}
			}

			{
				Cursor xyCursor = yCursor;

				while(xRight < width && *xyCursor == source)
				{
					*xyCursor = color;

					++xRight;
					xyCursor.xIncrement();
				}
			}

			// Recurse up and down.

			if (y < height - 1)
			{
				// Recurse top line.

				bool lineHandled = false;

				const integer yTop = y + 1;

				Cursor xyCursor = image.cursor(xLeft, yTop);

				for (integer x = xLeft;x < xRight;++x)
				{
					if (*xyCursor == source)
					{
						if (!lineHandled)
						{
							stack.push_back(Integer2(x, yTop));
							lineHandled = true;
						}
					}
					else
					{
						lineHandled = false;
					}

					xyCursor.xIncrement();
				}
			}
			if (y > 0)
			{
				// Recurse bottom line.

				bool lineHandled = false;

				const integer yBottom = y - 1;

				Cursor xyCursor = image.cursor(xLeft, yBottom);

				for (integer x = xLeft;x < xRight;++x)
				{
					if (*xyCursor == source)
					{
						if (!lineHandled)
						{
							stack.push_back(Integer2(x, yBottom));
							lineHandled = true;
						}
					}
					else
					{
						lineHandled = false;
					}

					xyCursor.xIncrement();
				}
			}
		}
	}

	namespace Detail_DrawView
	{

		template <typename ColorMixer>
		class Visitor
		{
		public:
			explicit Visitor(
				const ColorMixer& colorMixer)
				: colorMixer_(colorMixer)
			{
			}

			template <typename Texture, typename Image>
			void operator()(
				const Texture& texture,
				Image& image) const
			{
				image = colorMixer_(image, texture);
			}

		private:
			const ColorMixer& colorMixer_;
		};

	}

	template <int N,
		typename Texture_Element, typename Texture_View,
		typename Image_Element, typename Image_View,
		typename ColorMixer>
	void drawView(
		const ConstView<N, Texture_Element, Texture_View>& texture,
		const Point<N, integer>& position,
		const View<N, Image_Element, Image_View>& image,
		const ColorMixer& colorMixer)
	{
		const Rectangle<N> textureBox(
			position, position + texture.extent());
		const Rectangle<N> imageBox(
			Point<N, integer>(0), asPoint(image.extent()));

		Rectangle<N> clippedTextureBox;
		if (!intersect(textureBox, imageBox, clippedTextureBox))
		{
			return;
		}

		Detail_DrawView::Visitor<ColorMixer> visitor(colorMixer);

		visit(constSubView(texture, clippedTextureBox - asVector(position)),
			subView(image, clippedTextureBox), visitor);
	}

	template <int N,
		typename Texture_Element, typename Texture_View,
		typename Image_Element, typename Image_View>
	void drawView(
		const ConstView<N, Texture_Element, Texture_View>& texture,
		const Point<N, integer>& position,
		const View<N, Image_Element, Image_View>& image)
	{
		drawView(texture, position, image, assignColorMixer<Image_Element>());
	}

	template <
		typename Image_Element,
		typename Image_View,
		typename ColorMixer>
	void drawTexturedBox(
		const AlignedBox2& box,
		const Texture<Image_Element>& textureSampler,
		const View<2, Image_Element, Image_View>& image,
		const AlignedBox2& textureBox,
		const ColorMixer& colorMixer)
	{
		const Vector2 delta = box.extent();

		const Rectangle2 discreteBox(
			toPixelSpanPoint(box.min()), 
			toPixelSpanPoint(box.max()));

		const Rectangle2 window(
			IPoint2(0), 
			asPoint(image.extent()));

		Rectangle2 clippedBox;
		if (!intersect(discreteBox, window, clippedBox))
		{
			return;
		}

		const Vector2 textureDelta = textureBox.extent();
		const Vector2 duvDxy = textureDelta / delta;
		const Point2 uvMin = textureBox.min() + 
			((Point2(discreteBox.min()) + 0.5) - box.min()) * duvDxy;

		Point2 uv(uvMin);
		Vector2 duvDx(duvDxy.x(), 0);
		Vector2 duvDy(0, duvDxy.y());

		typedef typename Image_View::Cursor Cursor;

		Cursor yCursor = image.cursor(clippedBox.min());

		const integer width = clippedBox.extent().x();
		const integer height = clippedBox.extent().y();

		for (integer y = 0; y < height;++y)
		{
			Cursor xyCursor = yCursor;

			uv.x() = uvMin.x();

			for (integer x = 0; x < width;++x)
			{
				*xyCursor = colorMixer(*xyCursor,
					textureSampler(uv, duvDx, duvDy));

				uv.x() += duvDxy.x();
				xyCursor.xIncrement();
			}

			uv.y() += duvDxy.y();
			yCursor.yIncrement();
		}
	}

	template <
		typename Image_Element,
		typename Image_View>
	void drawTexturedBox(
		const AlignedBox2& box,
		const Texture<Image_Element>& textureSampler,
		const View<2, Image_Element, Image_View>& image,
		const AlignedBox2& textureBox)
	{
		drawTexturedBox(box, textureSampler, image, textureBox, assignColorMixer<Image_Element>());
	}

	template <
		typename Image_Element,
		typename Image_View>
	void drawTexturedBox(
		const AlignedBox2& box,
		const Texture<Image_Element>& textureSampler,
		const View<2, Image_Element, Image_View>& image)
	{
		drawTexturedBox(box, textureSampler, image, AlignedBox2(0, 0, 1, 1));
	}

}

#include "pastel/gfx/drawing_more.hpp"
#include "pastel/gfx/drawing_more2.hpp"

#endif
