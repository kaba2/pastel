#ifndef PASTEL_BOX_DIAMETER_HPP
#define PASTEL_BOX_DIAMETER_HPP

#include "pastel/geometry/box_diameter.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Real diameter2(
		const Box<Real, N>& box)
	{
		return 2 * dot(box.width());
	}

}

#endif
