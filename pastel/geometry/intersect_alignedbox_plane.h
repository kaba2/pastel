#ifndef PASTEL_INTERSECT_ALIGNEDBOX_PLANE_H
#define PASTEL_INTERSECT_ALIGNEDBOX_PLANE_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	template <int N, typename Real>
	AlignedBox<N, Real> intersect(
		const AlignedBox<N, Real>& box,
		const Plane<N, Real>& plane);

}

#include "pastel/geometry/intersect_alignedbox_plane.hpp"

#endif