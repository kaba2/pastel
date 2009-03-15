#include "pastel/dsp/lanczos_filter.h"

#include "pastel/sys/mathfunctions.h"

#include <cmath>

namespace Pastel
{

	LanczosFilter::LanczosFilter(real radius)
		: Filter(radius, "lanczos")
		, invRadius_(inverse(radius))
	{
	}

	LanczosFilter::~LanczosFilter()
	{
	}

	real LanczosFilter::evaluateInRange(real x) const
	{
		return sinc(x) * sinc(x * invRadius_);
	}

	void LanczosFilter::onSetRadius()
	{
		invRadius_ = inverse(radius());
	}

}
