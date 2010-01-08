#ifndef PASTEL_BOUNDING_SPHERE_SEGMENT_HPP
#define PASTEL_BOUNDING_SPHERE_SEGMENT_HPP

#include "pastel/geometry/bounding_sphere_segment.h"
#include "pastel/geometry/bounding_sphere_pointset.h"

namespace Pastel
{

	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const Segment<Real, N>& segment)
	{
		return Pastel::boundingSphere(segment.start(), segment.end());
	}

}

#endif
