// Description: Palette generation for indexed color systems

#ifndef PASTEL_PALETTE_H
#define PASTEL_PALETTE_H

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

	PASTELGFX void grayscalePalette(std::vector<Color>& palette, integer count = 256);

	//! Generates a palette good for false color images.

	PASTELGFX void smoothFalseColorPalette(std::vector<Color>& palette,
		integer count = 256);

	//! Generates a palette good for false color images.

	PASTELGFX void falseColorPalette(std::vector<Color>& palette,
		integer count = 256, integer seed = 42);

}

#endif
