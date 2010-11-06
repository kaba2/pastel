// Description: Intersection between two aligned boxes
// Documentation: alignedbox.txt

#ifndef PASTEL_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTEL_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/sys/alignedbox.h"

namespace Pastel
{

	template <typename Real, int N>
	bool intersect(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		AlignedBox<Real, N>& result);

}

#include "pastel/sys/intersect_alignedbox_alignedbox.hpp"

#endif
