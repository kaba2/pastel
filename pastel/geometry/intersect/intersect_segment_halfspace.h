// Description: Intersection between a line segment and a half-space

#ifndef PASTELGEOMETRY_INTERSECT_SEGMENT_HALFSPACE_H
#define PASTELGEOMETRY_INTERSECT_SEGMENT_HALFSPACE_H

#include "pastel/geometry/shapes/segment.h"
#include "pastel/geometry/shapes/plane.h"

namespace Pastel
{
	
	template <typename Real, int N>
	bool intersect(
		const Segment<Real, N>& segment,
		const Plane<Real, N>& halfSpace,
		Real& tMin,
		Real& tMax);

}

#include "pastel/geometry/intersect/intersect_segment_halfspace.hpp"

#endif
