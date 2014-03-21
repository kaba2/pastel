#include "pastel/gfx/color_illuminant.h"
#include "pastel/gfx/color_space.h"

namespace Pastel
{

	// Standard illuminants (CIE 1931)

	PASTELGFX Color xyzIlluminantE()
	{
		PASTEL_CONSTEXPR Color color(
			xyyToXyz(Color(
			(real32)1 / 3, (real32)1 / 3, 1)));

		return color;
	}

	PASTELGFX Color xyzIlluminantD50()
	{
		PASTEL_CONSTEXPR Color color(
			xyyToXyz(Color(
			0.34567, 0.35850, 1)));

		return color;
	}

	PASTELGFX Color xyzIlluminantD65()
	{
		PASTEL_CONSTEXPR Color color(
			xyyToXyz(Color(
			0.31271, 0.32902, 1)));

		return color;
	}

}
