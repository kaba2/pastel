// Description: Algorithms for rectangles
// Detail: intersect, etc.
// Documentation: math.txt

#ifndef PASTEL_RECTANGLE_TOOLS_H
#define PASTEL_RECTANGLE_TOOLS_H

#include "pastel/sys/rectangle.h"

namespace Pastel
{

	template <int N>
	bool intersect(
		const Rectangle<N>& aBox,
		const Rectangle<N>& bBox,
		Rectangle<N>& result);

}

#include "pastel/sys/rectangle_tools.hpp"

#endif
