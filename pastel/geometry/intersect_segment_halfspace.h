#ifndef PASTEL_INTERSECT_SEGMENT_HALFSPACE_H
#define PASTEL_INTERSECT_SEGMENT_HALFSPACE_H

#include "pastel/geometry/segment.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{
	
	template <int N, typename Real>
	bool intersect(
		const Segment<N, Real>& segment,
		const Plane<N, Real>& halfSpace,
		Real& tMin,
		Real& tMax);

}

#include "pastel/geometry/intersect_segment_halfspace.hpp"

#endif
