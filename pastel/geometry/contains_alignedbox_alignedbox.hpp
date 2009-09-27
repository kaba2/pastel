#ifndef PASTEL_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTEL_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/geometry/contains_alignedbox_alignedbox.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	bool contains(
		const AlignedBox<Real, N>& outer,
		const AlignedBox<Real, N>& inner)
	{
		return allGreaterEqual(
			inner.min(),
			outer.min()) &&
			allLessEqual(
			inner.max(),
			outer.max());
	}

}

#endif
