#include "pastel/dsp/gaussian_filter.h"

#include <cmath>

namespace Pastel
{

	GaussianFilter::GaussianFilter(real radius)
		: Filter(radius, "gaussian")
	{
	}

	GaussianFilter::~GaussianFilter()
	{
	}

	real GaussianFilter::evaluateInRange(real x) const
	{
		// I chose the factor -2 because
		// it fitted well with the lobe of
		// the Lanczos filter. No deeper
		// science here.

		return std::exp(-2 * x * x);
	}

}
