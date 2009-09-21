// Description: Diameter of a box
// Documentation: diameter.txt

#ifndef PASTEL_BOX_DIAMETER_H
#define PASTEL_BOX_DIAMETER_H

#include "pastel/geometry/box.h"

namespace Pastel
{

	//! Returns the squared diameter of a box.

	template <int N, typename Real>
	Real diameter2(
		const Box<Real, N>& box);

}

#include "pastel/geometry/box_diameter.hpp"

#endif
