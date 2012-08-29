// Description: Distance between an aligned box and a sphere

#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_SPHERE_H
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Computes the distance between an aligned box and a sphere.
	/*!
	Preconditions:
	box.dimension() == sphere.dimension()

	Time complexity:
	O(n), where n is the dimension.
	*/
	template <typename Real, int N>
	Real distance(
		const AlignedBox<Real, N>& box,
		const Sphere<Real, N>& sphere);

	//! Computes the squared distance between an aligned box and a sphere.
	/*!
	This is a convenience function that calls:

	square(distance(box, sphere))
	*/
	template <typename Real, int N>
	Real distance2(
		const AlignedBox<Real, N>& box,
		const Sphere<Real, N>& sphere);

}

#include "distance_alignedbox_sphere.hpp"

#endif
