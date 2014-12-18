// Description: Does a sphere contain an aligned box?

#ifndef PASTELGEOMETRY_CONTAINS_SPHERE_ALIGNEDBOX_H
#define PASTELGEOMETRY_CONTAINS_SPHERE_ALIGNEDBOX_H

#include "pastel/geometry/shapes/alignedbox.h"
#include "pastel/geometry/shapes/sphere.h"

namespace Pastel
{

	//! Finds out if an aligned box is contained in a sphere.
	template <typename Real, int N>
	bool contains(
		const Sphere<Real, N>& outerSphere,
		const AlignedBox<Real, N>& innerBox);

}

#include "pastel/geometry/contains/contains_sphere_alignedbox.hpp"

#endif
