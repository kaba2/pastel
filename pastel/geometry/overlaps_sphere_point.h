/*!
\file
\brief A function for testing overlap between a sphere and a point.
*/

#ifndef PASTEL_OVERLAPS_SPHERE_POINT_H
#define PASTEL_OVERLAPS_SPHERE_POINT_H

#include "pastel/geometry/sphere.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	//! Tests if a sphere and a point overlap.

	/*!
	The sphere is considered to be closed and solid.
	*/

	template <int N, typename Real>
	bool overlaps(const Sphere<N, Real>& sphere,
		const Point<N, Real>& point);

}

#include "pastel/geometry/overlaps_sphere_point.hpp"

#endif
