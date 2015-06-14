// Description: Does an aligned box contain a sphere?

#ifndef PASTELGEOMETRY_CONTAINS_ALIGNEDBOX_SPHERE_H
#define PASTELGEOMETRY_CONTAINS_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/sphere.h"

namespace Pastel
{

	//! Finds out if a sphere is contained in an aligned box.
	template <typename Real, integer N>
	bool contains(
		const AlignedBox<Real, N>& outerBox,
		const Sphere<Real, N>& innerSphere);

}

#include "pastel/geometry/containment/contains_alignedbox_sphere.hpp"

#endif
