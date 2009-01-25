#ifndef PASTELSYS_RECTANGLE_TOOLS_HPP
#define PASTELSYS_RECTANGLE_TOOLS_HPP

#include "pastel/sys/rectangle_tools.h"

#include "pastel/sys/point_tools.h"

namespace Pastel
{

	template <int N>
	bool intersect(
		const Rectangle<N>& aBox,
		const Rectangle<N>& bBox,
		Rectangle<N>& result)
	{
		for (integer i = 0;i < N;++i)
		{
			if (aBox.max()[i] < bBox.min()[i] ||
				bBox.max()[i] < aBox.min()[i])
			{
				return false;
			}
		}

		result.set(
			max(aBox.min(), bBox.min()),
			min(aBox.max(), bBox.max()));

		return true;
	}

}

#endif
