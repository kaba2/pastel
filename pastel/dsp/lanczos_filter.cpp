#include "pastel/dsp/lanczos_filter.h"

#include "pastel/sys/math_functions.h"

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
		return sinc<real>(x) * sinc<real>(x * invRadius_);
	}

	void LanczosFilter::onSetRadius()
	{
		invRadius_ = inverse(radius());
	}

}
