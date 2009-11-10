#ifndef PASTEL_BOUNDING_ALIGNEDBOX_POINTSET_HPP
#define PASTEL_BOUNDING_ALIGNEDBOX_POINTSET_HPP

#include "pastel/geometry/bounding_alignedbox_pointset.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real, int N, typename InputIterator>
	AlignedBox<Real, N> boundingAlignedBox(
		integer dimension,
		const InputIterator& from,
		const InputIterator& to)
	{
		PENSURE1((N == Dynamic && dimension > 0) ||
			(N != Dynamic && dimension == N), dimension);

		AlignedBox<Real, N> result(dimension);

		if (from != to)
		{
			PENSURE_OP(dimension, ==, from->dimension());

			InputIterator iter(from);
			while (iter != to)
			{
				extendToCover(*iter, result);

				++iter;
			}
		}

		return result;
	}

}

#endif
