// Description: Diameter of a box
// Documentation: diameter.txt

#ifndef PASTELGEOMETRY_BOX_DIAMETER_H
#define PASTELGEOMETRY_BOX_DIAMETER_H

#include "pastel/geometry/shape/box.h"

namespace Pastel
{

	//! Returns the squared diameter of a box.

	template <typename Real, integer N>
	Real diameter2(
		const Box<Real, N>& box);

}

#include "pastel/geometry/diameter/box_diameter.hpp"

#endif
