// Description: Overlap tests between a 2-flat and a plane

#ifndef PASTELGEOMETRY_OVERLAPS_FLAT_PLANE_H
#define PASTELGEOMETRY_OVERLAPS_FLAT_PLANE_H

#include "pastel/geometry/shapes/flat.h"
#include "pastel/geometry/shapes/plane.h"
#include "pastel/geometry/shapes/line.h"

namespace Pastel
{

	//! Tests if a 2-flat and a hyperplane overlap.
	template <typename Real, int N>
	bool overlaps(
		const Flat<Real, N, 2>& aPlane,
		const Plane<Real, N>& bPlane);

}

#include "pastel/geometry/overlaps/overlaps_flat_plane.hpp"

#endif
