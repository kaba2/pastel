#include "pastel/gfx/color_lch.h"

#include "pastel/math/coordinates.h"

namespace Pastel
{

	PASTELGFX Color labToLch(const Color& lab)
	{
		const Color shuffled = Color(lab[1], lab[2], lab[0]);

		const Color cylinder(cartesianToCylinder(shuffled));
		return Color(cylinder[2], cylinder[0], cylinder[1]);
	}

	PASTELGFX Color lchToLab(const Color& lch)
	{
		const Color cylinder = Color(lch[1], lch[2], lch[0]);
		const Color shuffled(cylinderToCartesian(cylinder));

		return Color(shuffled[2], shuffled[0], shuffled[1]);
	}

}
