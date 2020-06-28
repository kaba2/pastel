// Description: Distance between an aligned box and a sphere

#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_SPHERE_H
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/sphere.h"

#include "pastel/geometry/distance/distance_alignedbox_point.h"
#include "pastel/math/norm/euclidean_norm.h"

namespace Pastel
{

	//! Euclidean distance between a box and a sphere.
	/*!
	Preconditions:
	box.n() == sphere.n()

	Time complexity: O(box.n())

	The box and the sphere are assumed to be solid.
	*/
	template <typename Real, int N>
	auto distance2(
		const AlignedBox<Real, N>& box,
		const Sphere<Real, N>& sphere)
	{
		PENSURE_OP(box.n(), ==, sphere.n());

		// Let
		// s = distance from sphere's center to 'box'.
		// r = sphere's radius
		// Then the distance d is given by
		// d = s - r
		// Negative distance means overlapping,
		// so then zero distance is returned.

		auto norm = Euclidean_Norm<Real>();
		auto centerDistance = distance2(box, sphere.position());
		if (~centerDistance <= square(sphere.radius())) return norm();

		return norm((Real)centerDistance - sphere.radius());
	}

}

#endif
