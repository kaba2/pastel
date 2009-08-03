#ifndef PASTEL_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTEL_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
	bool intersect(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		AlignedBox<Real, N>& result);

}

#include "pastel/geometry/intersect_alignedbox_alignedbox.hpp"

#endif
