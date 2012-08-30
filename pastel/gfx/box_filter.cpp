#include "pastel/gfx/box_filter.h"

namespace Pastel
{

	Box_Filter::Box_Filter()
		: Filter(0.5, "box")
	{
	}

	Box_Filter::~Box_Filter()
	{
	}

	real Box_Filter::evaluateInRange(real x) const
	{
		return 1;
	}

}
