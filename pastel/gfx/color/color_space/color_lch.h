// Description: CIE-Lch color space
// Detail: Lightness-Chroma-Hue, CIE-Lab in cylindrical coordinates.
// Documentation: color_space.txt

#ifndef PASTELGFX_COLOR_LCH_H
#define PASTELGFX_COLOR_LCH_H

#include "pastel/gfx/color/color.h"

#include "pastel/math/coordinate/coordinates.h"

namespace Pastel
{

	//! Converts CIELab color to CIELch color.
	inline Color labToLch(const Color& lab)
	{
		Color shuffled = Color(lab[1], lab[2], lab[0]);

		Color cylinder(cartesianToCylinder(shuffled));
		return Color(cylinder[2], cylinder[0], cylinder[1]);
	}

	//! Converts CIELch color to CIELab color.
	inline Color lchToLab(const Color& lch)
	{
		Color cylinder = Color(lch[1], lch[2], lch[0]);
		Color shuffled(cylinderToCartesian(cylinder));

		return Color(shuffled[2], shuffled[0], shuffled[1]);
	}

}

#endif
