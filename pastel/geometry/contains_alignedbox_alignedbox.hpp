#ifndef PASTELGEOMETRY_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/geometry/contains_alignedbox_alignedbox.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	bool contains(
		const AlignedBox<N, Real>& outer,
		const AlignedBox<N, Real>& inner)
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
