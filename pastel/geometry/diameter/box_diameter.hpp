#ifndef PASTELGEOMETRY_BOX_DIAMETER_HPP
#define PASTELGEOMETRY_BOX_DIAMETER_HPP

#include "pastel/geometry/diameter/box_diameter.h"

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <typename Real, integer N>
	Real diameter2(
		const Box<Real, N>& box)
	{
		return 4 * dot(box.width());
	}

}

#endif
