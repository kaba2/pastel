#ifndef PASTEL_ALIGNEDBOX_DIAMETER_HPP
#define PASTEL_ALIGNEDBOX_DIAMETER_HPP

#include "pastel/geometry/alignedbox_diameter.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Real diameter2(
		const AlignedBox<Real, N>& box)
	{
		return dot(box.extent());
	}

}

#endif
