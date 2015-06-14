// Description: Bounding sphere of an aligned box

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_ALIGNEDBOX_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_ALIGNEDBOX_H

#include "pastel/geometry/shape/sphere.h"
#include "pastel/geometry/shape/alignedbox.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of an aligned box.

	template <typename Real, integer N>
	Sphere<Real, N> boundingSphere(
		const AlignedBox<Real, N>& alignedBox);

}

#include "pastel/geometry/bounding/bounding_sphere_alignedbox.hpp"

#endif
