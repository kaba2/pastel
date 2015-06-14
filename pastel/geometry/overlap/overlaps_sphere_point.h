// Description: Overlap tests between a sphere and a point

#ifndef PASTELGEOMETRY_OVERLAPS_SPHERE_POINT_H
#define PASTELGEOMETRY_OVERLAPS_SPHERE_POINT_H

#include "pastel/geometry/shape/sphere.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Tests if a sphere and a point overlap.
	/*!
	Preconditions:
	sphere.n() == point.n()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	Whether the sphere and the point overlap.
	*/
	template <typename Real, integer N>
	bool overlaps(
		const Sphere<Real, N>& sphere,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/overlap/overlaps_sphere_point.hpp"

#endif
