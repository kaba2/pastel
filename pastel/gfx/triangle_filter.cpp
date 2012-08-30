#include "pastel/gfx/triangle_filter.h"

#include <cmath>

namespace Pastel
{

	Triangle_Filter::Triangle_Filter()
		: Filter(1, "triangle")
	{
	}

	Triangle_Filter::~Triangle_Filter()
	{
	}

	real Triangle_Filter::evaluateInRange(real x) const
	{
		return 1 - mabs(x);
	}

}
