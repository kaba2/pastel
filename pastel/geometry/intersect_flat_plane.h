// Description: The intersection between a 2-flat and a plane

#ifndef PASTEL_INTERSECT_FLAT_PLANE_H
#define PASTEL_INTERSECT_FLAT_PLANE_H

#include "pastel/geometry/flat.h"
#include "pastel/geometry/plane.h"
#include "pastel/geometry/line.h"

namespace Pastel
{

	//! Finds the intersection between a 2-flat and a hyperplane.

	template <int N, typename Real>
	bool intersect(
		const Flat<Real, N, 2>& aPlane,
		const Plane<Real, N>& bPlane,
		Line<Real, N>& line);

}

#include "pastel/geometry/intersect_flat_plane.hpp"

#endif
