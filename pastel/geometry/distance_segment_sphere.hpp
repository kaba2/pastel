#ifndef PASTEL_DISTANCE_SEGMENT_SPHERE_HPP
#define PASTEL_DISTANCE_SEGMENT_SPHERE_HPP

#include "pastel/geometry/distance_segment_sphere.h"
#include "pastel/geometry/distance_segment_point.h"

#include <cmath>

namespace Pastel
{

	template <typename Real, int N>
	Real distance2(
		const Segment<Real, N>& segment,
		const Sphere<Real, N>& sphere)
	{
		PENSURE_OP(segment.dimension(), ==, sphere.dimension());

		const Real centerDistance2 =
			Pastel::distance2(segment, sphere.position());

		if (centerDistance2 <= square(sphere.radius()))
		{
			return 0;
		}

		return square(std::sqrt(centerDistance2) - sphere.radius());
	}

}

#endif
