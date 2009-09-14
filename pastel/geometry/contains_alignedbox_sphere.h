// Description: Does an aligned box contain a sphere?

#ifndef PASTEL_CONTAINS_ALIGNEDBOX_SPHERE_H
#define PASTEL_CONTAINS_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Finds out if a sphere is contained in an aligned box.
	template <int N, typename Real>
	bool contains(
		const AlignedBox<Real, N>& outerBox,
		const Sphere<Real, N>& innerSphere)

}

#include "pastel/geometry/contains_alignedbox_sphere.hpp"

#endif
