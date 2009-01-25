#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_SPHERE_H
#define PASTELGEOMETRY_DISTANCE_SEGMENT_SPHERE_H

#include "pastel/geometry/segment.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(
		const Segment<N, Real>& segment,
		const Sphere<N, Real>& sphere);

}

#include "pastel/geometry/distance_segment_sphere.hpp"

#endif
