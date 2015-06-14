#ifndef PASTELGEOMETRY_BOUNDING_BOX_HPP
#define PASTELGEOMETRY_BOUNDING_BOX_HPP

#include "pastel/geometry/bounding/bounding_box.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, integer N>
	Box<Real, N> boundingBox(const AlignedBox<Real, N>& alignedBox)
	{
		integer n = alignedBox.n();

		return Box<Real, N>(
			linear(alignedBox.min(), alignedBox.max(), 0.5),

			(alignedBox.max() - alignedBox.min()) * 0.5,
			identityMatrix<Real>(n));
	}

}

#endif
