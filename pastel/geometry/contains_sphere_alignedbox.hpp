#ifndef PASTELGEOMETRY_CONTAINS_SPHERE_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_CONTAINS_SPHERE_ALIGNEDBOX_HPP

#include "pastel/geometry/contains_sphere_alignedbox.h"

namespace Pastel
{

	template <typename Real, int N>
	bool contains(
		const Sphere<Real, N>& outerSphere,
		const AlignedBox<Real, N>& innerBox)
	{
		Vector<Real, N> boxCenter =
			linear(innerBox.min(), innerBox.max(), 0.5);

		const Vector<Real, N> boxRadius =
			innerBox.extent() / 2;

		const Real cornerDistance2 =
			dot(mabs(innerSphere.position() - boxCenter) + boxRadius);


		return cornerDistance2 <= innerSphere.radius() * innerSphere.radius();
	}

}

#endif
