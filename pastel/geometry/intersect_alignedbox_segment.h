#ifndef PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_SEGMENT_H
#define PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_SEGMENT_H

#include "pastel/geometry/segment.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
	bool intersect(
		const AlignedBox<N, Real>& alignedBox,
		const Segment<N, Real>& segment, 
		Segment<N, Real>& result);

}

#include "pastel/geometry/intersect_alignedbox_segment.hpp"

#endif
