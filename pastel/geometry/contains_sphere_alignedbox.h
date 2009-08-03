// Description: Containment tests between a sphere and an aligned box

#ifndef PASTEL_CONTAINS_SPHERE_ALIGNEDBOX_H
#define PASTEL_CONTAINS_SPHERE_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Finds out if an aligned box is contained in a sphere.
	template <int N, typename Real>
	bool contains(
		const Sphere<Real, N>& outerSphere,
		const AlignedBox<Real, N>& innerBox);

}

#include "pastel/geometry/contains_sphere_alignedbox.hpp"

#endif
