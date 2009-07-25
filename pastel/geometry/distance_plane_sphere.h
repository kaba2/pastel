// Description: Distance computation between a plane and a sphere

#ifndef PASTEL_DISTANCE_PLANE_SPHERE_H
#define PASTEL_DISTANCE_PLANE_SPHERE_H

#include "pastel/geometry/plane.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Computes the squared distance between a plane and a sphere.

	template <int N, typename Real>
	Real distance2(
		const Plane<N, Real>& plane,
		const Sphere<N, Real>& sphere);

}

#include "pastel/geometry/distance_plane_sphere.hpp"

#endif
