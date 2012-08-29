#ifndef PASTELGFX_DRAW_SEGMENT_HPP
#define PASTELGFX_DRAW_SEGMENT_HPP

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
