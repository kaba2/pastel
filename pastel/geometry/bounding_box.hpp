#ifndef PASTEL_BOUNDING_BOX_HPP
#define PASTEL_BOUNDING_BOX_HPP

#include "pastel/geometry/bounding_box.h"
#include "pastel/math/linear.h"

namespace Pastel
{

	template <int N, typename Real>
	Box<N, Real> boundingBox(const AlignedBox<N, Real>& alignedBox)
	{
		return Box<N, Real>(
			linear(alignedBox.min(), alignedBox.max(), 0.5),
			(alignedBox.max() - alignedBox.min()) * 0.5,
			Matrix<N, N, Real>());
	}

}

#endif
