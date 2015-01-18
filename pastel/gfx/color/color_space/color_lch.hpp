#ifndef PASTELGFX_COLOR_LCH_HPP
#define PASTELGFX_COLOR_LCH_HPP

#include "pastel/gfx/color/color_space/color_lch.h"

#include "pastel/math/coordinate/coordinates.h"

namespace Pastel
{

	inline Color labToLch(const Color& lab)
	{
		Color shuffled = Color(lab[1], lab[2], lab[0]);

		Color cylinder(cartesianToCylinder(shuffled));
		return Color(cylinder[2], cylinder[0], cylinder[1]);
	}


	inline Color lchToLab(const Color& lch)
	{
		Color cylinder = Color(lch[1], lch[2], lch[0]);
		Color shuffled(cylinderToCartesian(cylinder));

		return Color(shuffled[2], shuffled[0], shuffled[1]);
	}

}

#endif
