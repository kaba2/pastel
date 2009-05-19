#ifndef PASTEL_CONTAINS_ALIGNEDBOX_SPHERE_H
#define PASTEL_CONTAINS_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	template <int N, typename Real>
	bool contains(
		const AlignedBox<N, Real>& outerBox,
		const Sphere<N, Real>& innerSphere)

}

#include "pastel/geometry/contains_alignedbox_sphere.hpp"

#endif
