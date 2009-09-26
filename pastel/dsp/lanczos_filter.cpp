#include "pastel/dsp/lanczos_filter.h"

#include "pastel/sys/math_functions.h"

#include <cmath>

namespace Pastel
{

	Lanczos_Filter::Lanczos_Filter(real radius)
		: Filter(radius, "lanczos")
		, invRadius_(inverse(radius))
	{
	}

	Lanczos_Filter::~Lanczos_Filter()
	{
	}

	real Lanczos_Filter::evaluateInRange(real x) const
	{
		return sinc<real>(x) * sinc<real>(x * invRadius_);
	}

	void Lanczos_Filter::onSetRadius()
	{
		invRadius_ = inverse(radius());
	}

}
