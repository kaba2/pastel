// Description: Distance between a line segment and a sphere

#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_SPHERE_H
#define PASTELGEOMETRY_DISTANCE_SEGMENT_SPHERE_H

#include "pastel/geometry/segment.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Computes the squared distance between a line segment and a sphere.
	/*!
	Preconditions:
	segment.dimension() == sphere.dimension()

	Time complexity:
	O(n), where n is the dimension.
	*/
	template <typename Real, int N>
	Real distance2(
		const Segment<Real, N>& segment,
		const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/distance_segment_sphere.hpp"

#endif
