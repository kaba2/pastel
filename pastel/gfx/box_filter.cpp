#include "pastel/gfx/box_filter.h"

namespace Pastel
{

	BoxFilter::~BoxFilter()
	{
	}

	real BoxFilter::operator()(real x) const
	{
		if (-0.5 <= x && x < 0.5)
		{
			return 1;
		}

		return 0;
	}

	real BoxFilter::radius() const
	{
		return 0.5;
	}

	real BoxFilter::max() const
	{
		return 1;
	}

}
