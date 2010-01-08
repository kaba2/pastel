#ifndef PASTEL_BOUNDING_SPHERE_SEGMENT_H
#define PASTEL_BOUNDING_SPHERE_SEGMENT_H

#include "pastel/geometry/sphere.h"
#include "pastel/geometry/segment.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of a line segment.

	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const Segment<Real, N>& segment);

}

#include "pastel/geometry/bounding_sphere_segment.hpp"

#endif
