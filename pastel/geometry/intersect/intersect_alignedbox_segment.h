// Description: Intersection between an aligned box and a line segment

#ifndef PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_SEGMENT_H
#define PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_SEGMENT_H

#include "pastel/geometry/shape/segment.h"
#include "pastel/geometry/shape/alignedbox.h"

namespace Pastel
{

	template <typename Real, int N>
	bool intersect(
		const AlignedBox<Real, N>& alignedBox,
		const Segment<Real, N>& segment,
		Segment<Real, N>& result);

}

#include "pastel/geometry/intersect/intersect_alignedbox_segment.hpp"

#endif
