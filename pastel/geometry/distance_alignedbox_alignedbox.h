// Description: Distance between two aligned boxes

#ifndef PASTEL_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTEL_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox);

	template <typename Real, int N>
	Real farthestDistance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox);

}

#include "pastel/geometry/distance_alignedbox_alignedbox.hpp"

#endif
