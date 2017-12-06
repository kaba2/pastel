// Description: Distance between a line segment and a sphere

#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_SPHERE_H
#define PASTELGEOMETRY_DISTANCE_SEGMENT_SPHERE_H

#include "pastel/geometry/shape/segment.h"
#include "pastel/geometry/shape/sphere.h"
#include "pastel/geometry/distance/distance_segment_point.h"
#include "pastel/math/norm/euclidean_norm.h"

#include <cmath>

namespace Pastel
{

	//! Euclidean distance between a line segment and a sphere.
	/*!
	Preconditions:
	segment.n() == sphere.n()

	Time complexity: O(segment.n())
	*/
	template <typename Real, integer N>
	auto distance2(
		const Segment<Real, N>& segment,
		const Sphere<Real, N>& sphere)
	{
		PENSURE_OP(segment.n(), ==, sphere.n());
		
		auto norm = Euclidean_Norm<Real>();
		auto centerDistance = Pastel::distance2(segment, sphere.position());
		if (centerDistance <= sphere.radius()) return norm();
		
		return norm((Real)centerDistance - sphere.radius());
	}

}

#endif
