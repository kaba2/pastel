#ifndef PASTELGEOMETRY_ALIGNEDBOX_DIAMETER_HPP
#define PASTELGEOMETRY_ALIGNEDBOX_DIAMETER_HPP

#include "pastel/geometry/diameter/alignedbox_diameter.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Real diameter2(
		const AlignedBox<Real, N>& box)
	{
		return dot(box.extent());
	}

}

#endif
