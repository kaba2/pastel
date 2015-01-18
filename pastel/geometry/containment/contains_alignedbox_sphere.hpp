#ifndef PASTELGEOMETRY_CONTAINS_ALIGNEDBOX_SPHERE_H
#define PASTELGEOMETRY_CONTAINS_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/containment/contains_alignedbox_sphere.h"
#include "pastel/geometry/containment/contains_alignedbox_alignedbox.h"
#include "pastel/geometry/bounding/bounding_alignedbox.h"

namespace Pastel
{

	template <typename Real, int N>
	bool contains(
		const AlignedBox<Real, N>& outerBox,
		const Sphere<Real, N>& innerSphere)
	{
		return Pastel::contains(
			outerBox, boundingAlignedBox(innerSphere));
	}

}

#endif
