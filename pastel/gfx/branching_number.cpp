#include "pastel/gfx/branching_number.h"
#include "pastel/gfx/pixelneighbours.h"

#include <vector>

namespace Pastel
{

	PASTELGFX void branchingNumber(
		const Array<bool, 2>& image,
		Array<int32, 2>& numberImage)
	{
		// Let the 8-neighbourhood
		// of a binary pixel be enumerated as
		// follows:
		// d c b
		// e   a
		// f g h
		// The problem is to
		// count the number of connected
		// components in this ring
		// (number of chains of 1).
		// For example 10110101:
		// 0 1 0
		// 1   1
		// 1 0 1
		// Has three connected components (not 4).
		// This number is cached for each
		// 8-bit sequence for performance.

		// Cache the branching number table
		// if this is the first time.

		static std::vector<integer> table;
		static bool initialized = false;

		if (!initialized)
		{
			table.reserve(256);

			for (integer i = 0;i < 256;++i)
			{
				uint8 content = (uint8)i;
				integer number = 0;
				bool previousBit = (content & 0x80) != 0;
				uint8 bitMask = 1;
				for (integer j = 0;j < 8;++j)
				{
					const bool currentBit = ((content & bitMask) != 0);
					if (currentBit && !previousBit)
					{
						++number;
					}

					previousBit = currentBit;

					bitMask <<= 1;
				}

				table.push_back(number);
			}

			initialized = true;
		}

		integer width = image.width();
		integer height = image.height();

		Array<int32, 2> result(Vector2i(width, height), -1);

		// We want to avoid bound comparisons
		// so we treat the boundary pixels
		// specially.

		// Inner pixels

		for (integer x = 1; x < width - 1;++x)
		{
			for (integer y = 1; y < height - 1;++y)
			{
				if (image(x, y))
				{
					integer content = 0;
					if (image(x + 1, y))
					{
						content += 1;
					}
					if (image(x + 1, y + 1))
					{
						content += 2;
					}
					if (image(x, y + 1))
					{
						content += 4;
					}
					if (image(x - 1, y + 1))
					{
						content += 8;
					}
					if (image(x - 1, y))
					{
						content += 16;
					}
					if (image(x - 1, y - 1))
					{
						content += 32;
					}
					if (image(x, y - 1))
					{
						content += 64;
					}
					if (image(x + 1, y - 1))
					{
						content += 128;
					}

					result(x, y) = table[content];
				}
			}
		}

		// Bottom row

		if (height > 1)
		{
			integer y = 0;
			for (integer x = 1;x < width - 1;++x)
			{
				if (image(x, y))
				{
					integer content = 0;
					if (image(x + 1, y))
					{
						content += 1;
					}
					if (image(x + 1, y + 1))
					{
						content += 2;
					}
					if (image(x, y + 1))
					{
						content += 4;
					}
					if (image(x - 1, y + 1))
					{
						content += 8;
					}
					if (image(x - 1, y))
					{
						content += 16;
					}

					result(x, y) = table[content];
				}
			}
		}

		// Top row

		if (height > 1)
		{
			const integer y = height - 1;
			for (integer x = 1;x < width - 1;++x)
			{
				if (image(x, y))
				{
					integer content = 0;
					if (image(x + 1, y))
					{
						content += 1;
					}
					if (image(x - 1, y))
					{
						content += 16;
					}
					if (image(x - 1, y - 1))
					{
						content += 32;
					}
					if (image(x, y - 1))
					{
						content += 64;
					}
					if (image(x + 1, y - 1))
					{
						content += 128;
					}

					result(x, y) = table[content];
				}
			}
		}

		// Left column

		if (width > 1)
		{
			const integer x = 0;
			for (integer y = 1; y < height - 1;++y)
			{
				if (image(x, y))
				{
					integer content = 0;
					if (image(x + 1, y))
					{
						content += 1;
					}
					if (image(x + 1, y + 1))
					{
						content += 2;
					}
					if (image(x, y + 1))
					{
						content += 4;
					}
					if (image(x, y - 1))
					{
						content += 64;
					}
					if (image(x + 1, y - 1))
					{
						content += 128;
					}

					result(x, y) = table[content];
				}
			}
		}

		// Right column

		if (width > 1)
		{
			const integer x = width - 1;
			for (integer y = 1; y < height - 1;++y)
			{
				if (image(x, y))
				{
					integer content = 0;
					if (image(x, y + 1))
					{
						content += 4;
					}
					if (image(x - 1, y + 1))
					{
						content += 8;
					}
					if (image(x - 1, y))
					{
						content += 16;
					}
					if (image(x - 1, y - 1))
					{
						content += 32;
					}
					if (image(x, y - 1))
					{
						content += 64;
					}

					result(x, y) = table[content];
				}
			}
		}


		if (width > 1 && height > 1)
		{
			// Left-bottom corner
			{
				integer x = 0;
				integer y = 0;

				if (image(x, y))
				{
					integer content = 0;
					if (image(x + 1, y))
					{
						content += 1;
					}
					if (image(x + 1, y + 1))
					{
						content += 2;
					}
					if (image(x, y + 1))
					{
						content += 4;
					}

					result(x, y) = table[content];
				}
			}
			// Right-bottom corner
			{
				integer x = width - 1;
				const integer y = 0;

				if (image(x, y))
				{
					integer content = 0;
					if (image(x, y + 1))
					{
						content += 4;
					}
					if (image(x - 1, y + 1))
					{
						content += 8;
					}
					if (image(x - 1, y))
					{
						content += 16;
					}

					result(x, y) = table[content];
				}
			}
			// Right-top corner
			{
				const integer x = width - 1;
				const integer y = height - 1;

				if (image(x, y))
				{
					integer content = 0;
					if (image(x - 1, y))
					{
						content += 16;
					}
					if (image(x - 1, y - 1))
					{
						content += 32;
					}
					if (image(x, y - 1))
					{
						content += 64;
					}

					result(x, y) = table[content];
				}
			}
			// Left-top corner
			{
				const integer x = 0;
				const integer y = height - 1;

				if (image(x, y))
				{
					integer content = 0;
					if (image(x + 1, y))
					{
						content += 1;
					}
					if (image(x + 1, y + 1))
					{
						content += 2;
					}
					if (image(x, y + 1))
					{
						content += 4;
					}

					result(x, y) = table[content];
				}
			}
		}

		result.swap(numberImage);
	}

	PASTELGFX integer branchingNumber(

		const Array<bool, 2>& image,
		integer x, integer y)
	{
		integer width = image.width();
		integer height = image.height();


		ENSURE2(x >= 0 && x < width, x, width);
		ENSURE2(y >= 0 && y < height, y, height);

		uint32 neighbourMask = pixelNeighbourMask(image, x, y);

		integer result = 0;

		bool previousBit =

			(neighbourMask & 128) != 0;
		for (integer i = 0;i < 8;++i)
		{
			uint32 bitMask = 1 << i;
			bool currentBit =

				(neighbourMask & bitMask) != 0;
			if (currentBit && !previousBit)
			{
				++result;
			}

			previousBit = currentBit;
		}

		return result;
	}

	PASTELGFX uint32 branchingMask(
		const Array<bool, 2>& image,
		integer x, integer y)
	{
		integer width = image.width();
		integer height = image.height();


		ENSURE2(x >= 0 && x < width, x, width);
		ENSURE2(y >= 0 && y < height, y, height);

		uint32 neighbourMask = pixelNeighbourMask(image, x, y);

		uint32 resultMask = 0;

		bool previousBit =

			(neighbourMask & 128) != 0;
		for (integer i = 0;i < 8;++i)
		{
			uint32 bitMask = 1 << i;
			bool currentBit =

				(neighbourMask & bitMask) != 0;
			if (currentBit && !previousBit)
			{
				resultMask |= bitMask;
			}

			previousBit = currentBit;
		}

		// We prefer 4-neighbours to
		// 8-neighbours.

		for (integer i = 0;i < 8;++i)
		{
			uint32 bitMask = 1 << i;

			const uint32 nextMask = 1 << ((i + 1) & 7);
			bool nextBit =

				(neighbourMask & nextMask) != 0;
			const bool corner = (i & 1) != 0;

			if (corner && nextBit)
			{
				resultMask &= ~bitMask;

				resultMask |= nextMask;
			}
		}

		return resultMask;
	}

}
