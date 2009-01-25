#include "pastel/gfx/filterdesign.h"

namespace Pastel
{

	PASTELGFX integer designFilter(float filterFactor, float transitionWidth)
	{
		integer filterWidth = static_cast<integer>(filterFactor / transitionWidth);
		// Round up
		++filterWidth;

		// Round filterWidth to an odd number.
		filterWidth += ((filterWidth + 1) & 1);

		return filterWidth;
	}

	PASTELGFX integer designBox(float transitionWidth)
	{
		return designFilter(float(0.9), transitionWidth);
	}

	PASTELGFX integer designBartlett(float transitionWidth)
	{
		return designFilter(float(3.05), transitionWidth);
	}

	PASTELGFX integer designHanning(float transitionWidth)
	{
		return designFilter(float(3.1), transitionWidth);
	}

	PASTELGFX integer designHamming(float transitionWidth)
	{
		return designFilter(float(3.3), transitionWidth);
	}

	PASTELGFX integer designBlackman(float transitionWidth)
	{
		return designFilter(float(5.5), transitionWidth);
	}
}
