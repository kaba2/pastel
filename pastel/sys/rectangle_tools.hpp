#ifndef PASTEL_RECTANGLE_TOOLS_HPP
#define PASTEL_RECTANGLE_TOOLS_HPP

#include "pastel/sys/rectangle_tools.h"
#include "pastel/sys/vector_tools.h"

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
			if (aBox.max()[i] <= bBox.min()[i] ||
				bBox.max()[i] <= aBox.min()[i])
			{
				return false;
			}
		}

		result.set(
			max(aBox.min(), bBox.min()),
			min(aBox.max(), bBox.max()));

		return true;
	}

	template <int N, typename PositionVisitor>
	integer forEach(
		const Rectangle<N>& region,
		PositionVisitor positionVisitor)
	{
		integer visited = 0;

		if (that.empty())
		{
			return visited;
		}

		const integer n = region.dimension();
		Vector<integer, N> position(
			ofDimension(n), 0);
		integer i = 0;
		while(i < n)
		{
			++visited;
			if (!positionVisitor(position))
			{
				break;
			}

			i = 0;
			while(i < n)
			{
				++position[i];
				if (position[i] == region.extent()[i])
				{
					position[i] = 0;
				}
				else
				{
					break;
				}

				++i;
			}
		}
		
		return visited;
	}

}

#endif
