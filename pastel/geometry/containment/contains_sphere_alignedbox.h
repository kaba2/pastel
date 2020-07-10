// Description: Does a sphere contain an aligned box?

#ifndef PASTELGEOMETRY_CONTAINS_SPHERE_ALIGNEDBOX_H
#define PASTELGEOMETRY_CONTAINS_SPHERE_ALIGNEDBOX_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/sphere.h"

namespace Pastel
{

	//! Finds out if an aligned box is contained in a sphere.
	template <typename Real, int N>
	bool contains(
		const Sphere<Real, N>& outerSphere,
		const AlignedBox<Real, N>& innerBox)
	{
		Vector<Real, N> boxCenter =
			linear(innerBox.min(), innerBox.max(), 0.5);

		Vector<Real, N> boxRadius =
			innerBox.extent() / 2;

		Real cornerDistance2 =
			dot(abs(innerSphere.position() - boxCenter) + boxRadius);

		return cornerDistance2 <= innerSphere.radius() * innerSphere.radius();
	}

}

#endif
