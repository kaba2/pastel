// Description: Intersection between a plane and a sphere

#ifndef PASTEL_INTERSECT_PLANE_SPHERE_H
#define PASTEL_INTERSECT_PLANE_SPHERE_H

namespace Pastel
{

	//! Finds the intersection between a plane and a sphere.
	/*!
	The intersection of a sphere with a plane is a disk.
	We will use the sphere structure to return its center
	and radius. The disk's normal is the plane's normal.
	*/

	template <int N, typename Real>
		bool intersect(
			const Plane<Real, N>& plane,
			const Sphere<Real, N>& sphere,
			Sphere<Real, N>& result);

}

#include "pastel/geometry/intersect_plane_sphere.hpp"

#endif
