#ifndef PASTEL_DISTANCE_SEGMENT_SPHERE_HPP
#define PASTEL_DISTANCE_SEGMENT_SPHERE_HPP

#include "pastel/geometry/distance_segment_sphere.h"
#include "pastel/geometry/distance_segment_point.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(
		const Segment<Real, N>& segment,
		const Sphere<Real, N>& sphere)
	{
		const Real centerDistance2 =
			Pastel::distance2(segment, sphere.position());
		const Real radius2 =
			sphere.radius() * sphere.radius();

		if (centerDistance2 < radius2)
		{
			return 0;
		}

		// (sqrt(centerDistance2) - sphere.radius())^2
		// = centerDistance2 + radius2 -
		//   2 * sqrt(centerDistance2) * sphere.radius()

		return centerDistance2 + radius2 -
			2 * std::sqrt(centerDistance2) * sphere.radius();
	}

}

#endif
