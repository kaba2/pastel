#ifndef PASTELGFX_PACKEDCOLOR_H
#define PASTELGFX_PACKEDCOLOR_H

#include "pastel/sys/mytypes.h"

#include "pastel/gfx/color.h"

namespace Pastel
{

	//! Converts a color to a packed 32-bit x8r8g8b8 representation.

	uint32 colorToX8R8G8B8(const Color& color);

	//! Converts a packed 32-bit x8r8g8b8 representation to a color.

	Color colorFromX8R8G8B8(uint32 packedColor);

	//! Converts a color to a packed 16-bit r5g6b5 representation.

	uint32 colorToR5G6B5(const Color& color);

}

#include "pastel/gfx/packedcolor.hpp"

#endif
