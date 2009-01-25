/*!
\file
\brief Tools for working with rectangles.
*/

#ifndef PASTELSYS_RECTANGLE_TOOLS_H
#define PASTELSYS_RECTANGLE_TOOLS_H

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
