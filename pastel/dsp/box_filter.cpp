#include "pastel/dsp/box_filter.h"

namespace Pastel
{

	BoxFilter::BoxFilter()
		: Filter(0.5, "box")
	{
	}

	BoxFilter::~BoxFilter()
	{
	}

	real BoxFilter::evaluateInRange(real x) const
	{
		return 1;
	}

}
