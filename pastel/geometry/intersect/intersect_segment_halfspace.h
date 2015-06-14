// Description: Intersection between a line segment and a half-space

#ifndef PASTELGEOMETRY_INTERSECT_SEGMENT_HALFSPACE_H
#define PASTELGEOMETRY_INTERSECT_SEGMENT_HALFSPACE_H

#include "pastel/geometry/shape/segment.h"
#include "pastel/geometry/shape/plane.h"

namespace Pastel
{
	
	template <typename Real, integer N>
	bool intersect(
		const Segment<Real, N>& segment,
		const Plane<Real, N>& halfSpace,
		Real& tMin,
		Real& tMax);

}

#include "pastel/geometry/intersect/intersect_segment_halfspace.hpp"

#endif
