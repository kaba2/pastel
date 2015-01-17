#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_BOUNDING_SPHERE_ALIGNEDBOX_HPP

#include "pastel/geometry/bounding/bounding_sphere_alignedbox.h"

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const AlignedBox<Real, N>& alignedBox)
	{
		Vector<Real, N> delta = alignedBox.max() - alignedBox.min();
		return Sphere<Real, N>(
			linear(alignedBox.min(), alignedBox.max(), 0.5),

			std::sqrt(dot(delta)) * 0.5);
	}

}

#endif
