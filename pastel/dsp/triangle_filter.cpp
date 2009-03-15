#include "pastel/dsp/triangle_filter.h"

#include <cmath>

namespace Pastel
{

	TriangleFilter::TriangleFilter()
		: Filter(1, "triangle")
	{
	}

	TriangleFilter::~TriangleFilter()
	{
	}

	real TriangleFilter::evaluateInRange(real x) const
	{
		return 1 - std::abs(x);
	}

}
