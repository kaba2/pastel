// Description: Bounding sphere of a line segment

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_SEGMENT_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_SEGMENT_H

#include "pastel/geometry/shapes/sphere.h"
#include "pastel/geometry/shapes/segment.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of a line segment.

	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const Segment<Real, N>& segment);

}

#include "pastel/geometry/bounding/bounding_sphere_segment.hpp"

#endif
