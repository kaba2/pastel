#ifndef PASTEL_BOUNDING_BOX_HPP
#define PASTEL_BOUNDING_BOX_HPP

#include "pastel/geometry/bounding_box.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <int N, typename Real>
	Box<Real, N> boundingBox(const AlignedBox<Real, N>& alignedBox)
	{
		return Box<Real, N>(
			linear(alignedBox.min(), alignedBox.max(), 0.5),
			(alignedBox.max() - alignedBox.min()) * 0.5,
			Matrix<N, N, Real>());
	}

}

#endif
