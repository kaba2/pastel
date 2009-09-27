// Description: Overlap tests between a sphere and a point

#ifndef PASTEL_OVERLAPS_SPHERE_POINT_H
#define PASTEL_OVERLAPS_SPHERE_POINT_H

#include "pastel/geometry/sphere.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Tests if a sphere and a point overlap.

	/*!
	The sphere is considered to be closed and solid.
	*/

	template <typename Real, int N>
	bool overlaps(const Sphere<Real, N>& sphere,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/overlaps_sphere_point.hpp"

#endif
