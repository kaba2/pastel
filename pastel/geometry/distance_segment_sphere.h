// Description: Distance between a line segment and a sphere

#ifndef PASTEL_DISTANCE_SEGMENT_SPHERE_H
#define PASTEL_DISTANCE_SEGMENT_SPHERE_H

#include "pastel/geometry/segment.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(
		const Segment<Real, N>& segment,
		const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/distance_segment_sphere.hpp"

#endif
