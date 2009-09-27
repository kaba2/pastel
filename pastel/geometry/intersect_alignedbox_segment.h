// Description: Intersection between an aligned box and a line segment

#ifndef PASTEL_INTERSECT_ALIGNEDBOX_SEGMENT_H
#define PASTEL_INTERSECT_ALIGNEDBOX_SEGMENT_H

#include "pastel/geometry/segment.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <typename Real, int N>
	bool intersect(
		const AlignedBox<Real, N>& alignedBox,
		const Segment<Real, N>& segment,
		Segment<Real, N>& result);

}

#include "pastel/geometry/intersect_alignedbox_segment.hpp"

#endif
