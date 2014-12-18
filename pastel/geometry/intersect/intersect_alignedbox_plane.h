// Description: Intersection between an aligned box and a plane

#ifndef PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_PLANE_H
#define PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_PLANE_H

#include "pastel/geometry/shapes/alignedbox.h"
#include "pastel/geometry/shapes/plane.h"

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

#include "pastel/geometry/intersect/intersect_alignedbox_plane.hpp"

#endif
