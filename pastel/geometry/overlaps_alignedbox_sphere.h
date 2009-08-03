// Description: Overlap tests between an aligned box and a sphere

#ifndef PASTEL_OVERLAPS_ALIGNEDBOX_SPHERE_H
#define PASTEL_OVERLAPS_ALIGNEDBOX_SPHERE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Tests if an aligned box and a sphere overlap.

	/*!
	The aligned box and the sphere are considered closed and
	solid.
	*/

	template <int N, typename Real>
	bool overlaps(
		const AlignedBox<Real, N>& alignedBox,
		const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/overlaps_alignedbox_sphere.hpp"

#endif
