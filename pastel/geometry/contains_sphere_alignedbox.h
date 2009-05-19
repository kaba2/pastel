#ifndef PASTEL_CONTAINS_SPHERE_ALIGNEDBOX_H
#define PASTEL_CONTAINS_SPHERE_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	template <int N, typename Real>
	bool contains(
		const Sphere<N, Real>& outerSphere,
		const AlignedBox<N, Real>& innerBox);

}

#include "pastel/geometry/contains_sphere_alignedbox.hpp"

#endif
