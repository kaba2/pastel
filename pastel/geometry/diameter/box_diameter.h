// Description: Diameter of a box
// Documentation: diameter.txt

#ifndef PASTELGEOMETRY_BOX_DIAMETER_H
#define PASTELGEOMETRY_BOX_DIAMETER_H

#include "pastel/geometry/shape/box.h"
#include "pastel/sys/vector/vector.h"

namespace Pastel
{

	//! Returns the squared diameter of a box.
	template <typename Real, int N>
	Real diameter2(
		const Box<Real, N>& box)
	{
		return 4 * dot(box.width());
	}

}

#endif
