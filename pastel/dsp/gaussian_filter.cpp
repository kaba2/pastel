#include "pastel/dsp/gaussian_filter.h"

#include <cmath>

namespace Pastel
{

	GaussianFilter::GaussianFilter(real radius)
		: radius_(radius)
	{
	}

	GaussianFilter::~GaussianFilter()
	{
	}

	real GaussianFilter::operator()(real x) const
	{
		// NOTE: I chose A = -2 because
		// it fitted well with the lobe of
		// the Lanczos filter. No deeper
		// science here.

		static const real A = -2;

		if (std::abs(x) < radius_)
		{
			return std::exp(A * x * x);
		}

		return 0;
	}

	void GaussianFilter::setRadius(real radius)
	{
		radius_ = radius;
	}

	real GaussianFilter::radius() const
	{
		return radius_;
	}

	std::string GaussianFilter::name() const
	{
		return std::string("gaussian");
	}

}
