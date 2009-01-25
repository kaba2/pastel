#ifndef PASTELSYS_RECTANGLE_HPP
#define PASTELSYS_RECTANGLE_HPP

#include "pastel/sys/rectangle.h"

namespace Pastel
{

	template <int N>
	void swap(Rectangle<N>& left,
		Rectangle<N>& right)
	{
		left.swap(right);
	}

}

#endif
