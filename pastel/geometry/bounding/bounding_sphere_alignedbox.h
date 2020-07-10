// Description: Bounding sphere of an aligned box

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_ALIGNEDBOX_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_ALIGNEDBOX_H

#include "pastel/geometry/shape/sphere.h"
#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/sys/vector/vector.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of an aligned box.
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
