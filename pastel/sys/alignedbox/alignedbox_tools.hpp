#ifndef PASTELSYS_ALIGNEDBOX_TOOLS_HPP
#define PASTELSYS_ALIGNEDBOX_TOOLS_HPP

#include "pastel/sys/alignedbox_tools.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N>
	integer linearIndex(
		const Vector<integer, N>& point,
		const Vector<integer, N>& stride)
	{
		return dot(point, stride);
	}

	template <int N>
	Vector<integer, N> position(
		integer linearIndex,
		const Vector<integer, N>& stride,
		const Vector<integer, N>& order)
	{
		integer n = stride.n();

		Vector<integer, N> result(
			ofDimension(n));
		
		for (integer i = n - 1;i > 0;--i)
		{
			integer k = order[i];
			result[k] = linearIndex / stride[k];

			linearIndex -= result[k] * stride[k];
		}
		integer k = order[0];
		result[k] = linearIndex;

		return result;
	}

	template <int N>
	Vector<integer, N> position(
		integer linearIndex,

		const Vector<integer, N>& stride)
	{
		integer n = stride.n();

		Vector<integer, N> result(
			ofDimension(n));
		
		for (integer i = n - 1;i > 0;--i)
		{
			result[i] = linearIndex / stride[i];

			linearIndex -= result[i] * stride[i];
		}
		result[0] = linearIndex;

		return result;
	}

	template <int N, typename PositionVisitor>
	integer forEach(
		const AlignedBox<integer, N>& region,
		PositionVisitor positionVisitor)
	{
		integer visited = 0;

		if (region.empty())
		{
			return visited;
		}

		integer n = region.n();
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
			Pastel::discreteToContinuous(
			continuousRange, discreteRange, discreteBox.min()),
			Pastel::discreteToContinuous(
			continuousRange, discreteRange, discreteBox.max()));
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
