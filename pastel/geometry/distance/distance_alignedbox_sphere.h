// Description: Distance between an aligned box and a sphere

#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_SPHERE_H
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/shapes/alignedbox.h"
#include "pastel/geometry/shapes/sphere.h"

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
	Real distance(
		const AlignedBox<Real, N>& box,
		const Sphere<Real, N>& sphere);

	//! Squared Euclidean distance between a box and a sphere.
	/*!
	This is a convenience function which returns
	square(distance(box, sphere)).
	*/
	template <typename Real, int N>
	Real distance2(
		const AlignedBox<Real, N>& box,
		const Sphere<Real, N>& sphere);

}

#include "distance_alignedbox_sphere.hpp"

#endif
