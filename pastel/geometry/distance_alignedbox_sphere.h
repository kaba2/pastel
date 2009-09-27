// Description: Distance between an aligned box and a sphere

#ifndef PASTEL_DISTANCE_ALIGNEDBOX_SPHERE_H
#define PASTEL_DISTANCE_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Computes the squared distance between an aligned box and a sphere.

	template <typename Real, int N>
	Real distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Sphere<Real, N>& sphere);

}

#include "distance_alignedbox_sphere.hpp"

#endif
