#ifndef PASTEL_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTEL_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
	bool intersect(
		const AlignedBox<N, Real>& aBox,
		const AlignedBox<N, Real>& bBox,
		AlignedBox<N, Real>& result);

}

#include "pastel/geometry/intersect_alignedbox_alignedbox.hpp"

#endif
