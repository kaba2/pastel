// Description: Palettes for indexed color systems

#ifndef PASTELGFX_COLOR_PALETTE_H
#define PASTELGFX_COLOR_PALETTE_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

#include "pastel/sys/mytypes.h"

#include <vector>

namespace Pastel
{

	//! Generates the default vga palette.

	PASTELGFX void vgaPalette(std::vector<Color>& palette);

	//! Generates a random palette.

	PASTELGFX void randomPalette(std::vector<Color>& palette, integer count = 256);

	//! Generates a grayscale palette.
	/*!
	Preconditions:
	count >= 0
	maxValue >= 0
	*/

	PASTELGFX void grayscalePalette(std::vector<Color>& palette, integer count = 256, real32 maxValue = 1);

	//! Generates a palette good for false color images.

	PASTELGFX void smoothFalseColorPalette(std::vector<Color>& palette,
		integer count = 256);

	//! Generates a palette good for false color images.

	PASTELGFX void falseColorPalette(std::vector<Color>& palette,
		integer count = 256);

}

#endif
