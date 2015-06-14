// Description: Find the nearest standard basis axis for a given vector.
// Documentation: common.txt

#ifndef PASTELGEOMETRY_NEAREST_MAIN_AXIS_H
#define PASTELGEOMETRY_NEAREST_MAIN_AXIS_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Finds the nearest standard basis axis for a given vector.
	template <typename Real, integer N>
	integer nearestMainAxis(
		const Vector<Real, N>& that);

}

#include "pastel/geometry/nearest_main_axis.hpp"

#endif
