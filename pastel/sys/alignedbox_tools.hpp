#ifndef PASTEL_ALIGNEDBOX_TOOLS_HPP
#define PASTEL_ALIGNEDBOX_TOOLS_HPP

#include "pastel/sys/alignedbox_tools.h"

namespace Pastel
{

	template <int N, typename PositionVisitor>
	integer forEach(
		const AlignedBox<integer, N>& region,
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

	template <typename Real, int N>
	Vector<Real, N> discreteToContinuous(
		const AlignedBox<Real, N>& continuousRange,
		const AlignedBox<integer, N>& discreteRange,
		const Vector<integer, N>& discretePoint)
	{
		return continuousRange.min() +
			(Vector<Real, N>(discretePoint - discreteRange.min()) /
			Vector<Real, N>(discreteRange.extent())) *
			continuousRange.extent();
	}

	template <typename Real, int N>
	AlignedBox<Real, N> discreteToContinuous(
		const AlignedBox<Real, N>& continuousRange,
		const AlignedBox<integer, N>& discreteRange,
		const AlignedBox<integer, N>& discreteBox)
	{
		return AlignedBox<Real, N>(
			Pastel::discreteToContinuous(continuousRange, discreteRange, clippedBox.min()),
			Pastel::discreteToContinuous(continuousRange, discreteRange, clippedBox.max()));
	}

	template <typename Real, int N>
	Vector<integer, N> continuousToDiscrete(
		const AlignedBox<Real, N>& continuousRange,
		const AlignedBox<integer, N>& discreteRange,
		const Vector<Real, N>& continuousPoint)
	{
		return
			discreteRange.min() +
			Vector<integer, N>(floor(
			((continuousPoint - continuousRange.min()) / continuousRange.extent()) *
			Vector<Real, N>(discreteRange.extent())));
	}

	template <typename Real, int N>
	AlignedBox<integer, N> continuousToDiscrete(
		const AlignedBox<Real, N>& continuousRange,
		const AlignedBox<integer, N>& discreteRange,
		const AlignedBox<Real, N>& continuousBox)
	{
		return AlignedBox<integer, N>(
			Pastel::continuousToDiscrete(continuousRange, discreteRange, continuousBox.min()),
			Pastel::continuousToDiscrete(continuousRange, discreteRange, continuousBox.max()) + 1);
	}

}

#endif
