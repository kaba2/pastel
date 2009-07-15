// Description: Overlap tests between a 2-flat and a plane

#ifndef PASTEL_OVERLAPS_FLAT_PLANE_H
#define PASTEL_OVERLAPS_FLAT_PLANE_H

#include "pastel/geometry/flat.h"
#include "pastel/geometry/plane.h"
#include "pastel/geometry/line.h"

namespace Pastel
{

	//! Tests if a 2-flat and a hyperplane overlap.

	template <int N, typename Real>
	bool overlaps(
		const Flat<N, Real, 2>& aPlane,
		const Plane<N, Real>& bPlane);

}

#include "pastel/geometry/overlaps_flat_plane.hpp"

#endif
