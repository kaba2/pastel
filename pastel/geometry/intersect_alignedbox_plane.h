#ifndef PASTEL_INTERSECT_ALIGNEDBOX_PLANE_H
#define PASTEL_INTERSECT_ALIGNEDBOX_PLANE_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	template <int N, typename Real>
	bool intersect(
		const AlignedBox<Real, N>& box,
		const Plane<Real, N>& plane, 
		integer& clipDimension,
		PASTEL_NO_DEDUCTION(Real)& minBoxMax,
		PASTEL_NO_DEDUCTION(Real)& maxBoxMin);

}

#include "pastel/geometry/intersect_alignedbox_plane.hpp"

#endif
