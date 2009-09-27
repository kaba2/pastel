// Description: Overlap tests between a plane and a sphere

#ifndef PASTEL_OVERLAPS_PLANE_SPHERE_H
#define PASTEL_OVERLAPS_PLANE_SPHERE_H

#include "pastel/geometry/plane.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Tests if a plane and a sphere overlap.

	/*!
	The sphere is considered to be closed and solid.
	*/

	template <typename Real, int N>
		bool overlaps(
			const Plane<Real, N>& plane,
			const Sphere<Real, N>& sphere);

	//! Tests if a plane and a sphere overlap.

	/*!
	The sphere is considered to be closed and solid.
	'sphereOnPositiveSide' is filled with the information
	on which side of the plane the center of the sphere is.
	*/

	template <typename Real, int N>
		bool overlaps(
			const Plane<Real, N>& plane,
			const Sphere<Real, N>& sphere,
			bool& sphereOnPositiveSide);

}

#include "pastel/geometry/overlaps_plane_sphere.hpp"

#endif
