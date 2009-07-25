// Description: Distance computation between an aligned box and a sphere

#ifndef PASTEL_DISTANCE_ALIGNEDBOX_SPHERE_H
#define PASTEL_DISTANCE_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Computes the squared distance between an aligned box and a sphere.

	template <int N, typename Real>
	Real distance2(
		const AlignedBox<N, Real>& alignedBox,
		const Sphere<N, Real>& sphere);

}

#include "distance_alignedbox_sphere.hpp"

#endif
