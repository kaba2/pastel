// Description: Intersection between an aligned box and a plane

#ifndef PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_PLANE_H
#define PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_PLANE_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	template <typename Real, int N>
	bool intersect(
		const AlignedBox<Real, N>& box,
		const Plane<Real, N>& plane, 
		integer& clipDimension,
		NoDeduction<Real>& minBoxMax,
		NoDeduction<Real>& maxBoxMin);

}

#include "pastel/geometry/intersect_alignedbox_plane.hpp"

#endif
