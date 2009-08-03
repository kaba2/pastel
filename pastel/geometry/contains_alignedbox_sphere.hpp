#ifndef PASTEL_CONTAINS_ALIGNEDBOX_SPHERE_H
#define PASTEL_CONTAINS_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/contains_alignedbox_sphere.h"
#include "pastel/geometry/contains_alignedbox_alignedbox.h"
#include "pastel/geometry/bounding_alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
	bool contains(
		const AlignedBox<Real, N>& outerBox,
		const Sphere<Real, N>& innerSphere)
	{
		return Pastel::contains(
			outerBox, boundingAlignedBox(innerSphere));
	}

}

#endif
