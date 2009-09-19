// Description: CIE-Lch color space
// Detail: Lightness-Chroma-Hue, CIE-Lab in cylindrical coordinates.
// Documentation: color_space.txt

#ifndef PASTEL_COLOR_LCH_H
#define PASTEL_COLOR_LCH_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

namespace Pastel
{

	//! Converts CIELab color to CIELch color.
	PASTELGFX Color labToLch(const Color& lab);

	//! Converts CIELch color to CIELab color.
	PASTELGFX Color lchToLab(const Color& lch);

}

#endif
