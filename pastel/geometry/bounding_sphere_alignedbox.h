// Description: Bounding sphere of an aligned box

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_ALIGNEDBOX_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_ALIGNEDBOX_H

#include "pastel/geometry/sphere.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of an aligned box.

	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const AlignedBox<Real, N>& alignedBox);

}

#include "pastel/geometry/bounding_sphere_alignedbox.hpp"

#endif
