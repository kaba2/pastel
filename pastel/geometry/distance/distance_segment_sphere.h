// Description: Distance between a line segment and a sphere

#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_SPHERE_H
#define PASTELGEOMETRY_DISTANCE_SEGMENT_SPHERE_H

#include "pastel/geometry/segment.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Euclidean distance between a line segment and a sphere.
	/*!
	This is a convenience function which returns
	std::sqrt(distance2(segment, sphere)).
	*/
	template <typename Real, int N>
	Real distance(
		const Segment<Real, N>& segment,
		const Sphere<Real, N>& sphere);

	//! Squared Euclidean distance between a line segment and a sphere.
	/*!
	Preconditions:
	segment.n() == sphere.n()

	Time complexity: O(segment.n())
	*/
	template <typename Real, int N>
	Real distance2(
		const Segment<Real, N>& segment,
		const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/distance/distance_segment_sphere.hpp"

#endif
