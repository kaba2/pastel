// Description: Diameter of an aligned box
// Documentation: diameter.txt

#ifndef PASTELGEOMETRY_ALIGNEDBOX_DIAMETER_H
#define PASTELGEOMETRY_ALIGNEDBOX_DIAMETER_H

#include "pastel/geometry/shapes/alignedbox.h"

namespace Pastel
{

	//! Returns the squared diameter of an aligned box.

	template <typename Real, int N>
	Real diameter2(
		const AlignedBox<Real, N>& box);

}

#include "pastel/geometry/diameter/alignedbox_diameter.hpp"

#endif
