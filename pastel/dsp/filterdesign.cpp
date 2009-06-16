#include "pastel/dsp/filterdesign.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	PASTELDSP integer designFilter(float filterFactor, float transitionWidth)
	{
		integer filterWidth = static_cast<integer>(filterFactor / transitionWidth);
		// Round up
		++filterWidth;

		// Round filterWidth to an odd number.
		filterWidth = roundUpToOdd(filterWidth);

		return filterWidth;
	}

	PASTELDSP integer designBox(float transitionWidth)
	{
		return designFilter(float(0.9), transitionWidth);
	}

	PASTELDSP integer designBartlett(float transitionWidth)
	{
		return designFilter(float(3.05), transitionWidth);
	}

	PASTELDSP integer designHanning(float transitionWidth)
	{
		return designFilter(float(3.1), transitionWidth);
	}

	PASTELDSP integer designHamming(float transitionWidth)
	{
		return designFilter(float(3.3), transitionWidth);
	}

	PASTELDSP integer designBlackman(float transitionWidth)
	{
		return designFilter(float(5.5), transitionWidth);
	}
}
