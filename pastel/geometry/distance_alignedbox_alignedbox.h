#ifndef PASTEL_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTEL_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(
		const AlignedBox<N, Real>& aBox,
		const AlignedBox<N, Real>& bBox);

	template <int N, typename Real>
	Real farthestDistance2(
		const AlignedBox<N, Real>& aBox,
		const AlignedBox<N, Real>& bBox);

}

#include "pastel/geometry/distance_alignedbox_alignedbox.hpp"

#endif
