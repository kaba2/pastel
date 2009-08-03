#ifndef PASTEL_CONTAINS_SPHERE_ALIGNEDBOX_HPP
#define PASTEL_CONTAINS_SPHERE_ALIGNEDBOX_HPP

#include "pastel/geometry/contains_sphere_alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
	bool contains(
		const Sphere<N, Real>& outerSphere,
		const AlignedBox<N, Real>& innerBox)
	{
		const Point<Real, N> boxCenter =
			linear(innerBox.min(), innerBox.max(), 0.5);

		const Vector<Real, N> boxRadius =
			innerBox.extent() / 2;

		const Real cornerDistance2 =
			dot(mabs(innerSphere.position() - boxCenter) + boxRadius);

		return cornerDistance2 <= innerSphere.radius() * innerSphere.radius();
	}

}

#endif
