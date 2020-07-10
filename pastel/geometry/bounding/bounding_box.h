// Description: Bounding boxes

#ifndef PASTELGEOMETRY_BOUNDING_BOX_H
#define PASTELGEOMETRY_BOUNDING_BOX_H

#include "pastel/geometry/shape/box.h"
#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, int N>
	Box<Real, N> boundingBox(const AlignedBox<Real, N>& alignedBox)
	{
		integer n = alignedBox.n();

		return Box<Real, N>(
			linear(alignedBox.min(), alignedBox.max(), 0.5),

			(alignedBox.max() - alignedBox.min()) * 0.5,
			identityMatrix<Real, N, N>(n));
	}

}

#endif
