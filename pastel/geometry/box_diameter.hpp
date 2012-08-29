#ifndef PASTELGEOMETRY_BOX_DIAMETER_HPP
#define PASTELGEOMETRY_BOX_DIAMETER_HPP

#include "pastel/geometry/box_diameter.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Real diameter2(
		const Box<Real, N>& box)
	{
		return 4 * dot(box.width());
	}

}

#endif
