// Description: CIE-Lch color space
// Detail: Lightness-Chroma-Hue, CIE-Lab in cylindrical coordinates.
// Documentation: color_space.txt

#ifndef PASTELGFX_COLOR_LCH_H
#define PASTELGFX_COLOR_LCH_H

#include "pastel/gfx/color.h"

namespace Pastel
{

	//! Converts CIELab color to CIELch color.
	Color labToLch(const Color& lab);

	//! Converts CIELch color to CIELab color.
	Color lchToLab(const Color& lch);

}

#include "pastel/gfx/color_lch.hpp"

#endif
