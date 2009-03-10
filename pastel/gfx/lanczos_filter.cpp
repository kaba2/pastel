#include "pastel/gfx/lanczos_filter.h"

#include "pastel/sys/mathfunctions.h"

#include <cmath>

namespace Pastel
{

	LanczosFilter::LanczosFilter(real radius)
		: radius_(radius)
		, invRadius_(inverse(radius))
	{
	}

	LanczosFilter::~LanczosFilter()
	{
	}

	real LanczosFilter::operator()(real x) const
	{
		const real xAbs = std::abs(x);

		if (xAbs < radius_)
		{
			return sinc(xAbs) * sinc(xAbs * invRadius_);
		}

		return 0;
	}

	void LanczosFilter::setRadius(real radius)
	{
		radius_ = radius;
		invRadius_ = inverse(radius);
	}

	real LanczosFilter::radius() const
	{
		return radius_;
	}

	std::string LanczosFilter::name() const
	{
		return std::string("lanczos");
	}

}
