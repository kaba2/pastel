#include "pastel/dsp/triangle_filter.h"

#include <cmath>

namespace Pastel
{

	TriangleFilter::~TriangleFilter()
	{
	}

	real TriangleFilter::operator()(real x) const
	{
		const real xAbs = std::abs(x);

		if (xAbs < 1)
		{
			return 1 - xAbs;
		}

		return 0;
	}

	real TriangleFilter::radius() const
	{
		return 1;
	}

	std::string TriangleFilter::name() const
	{
		return std::string("triangle");
	}

}
