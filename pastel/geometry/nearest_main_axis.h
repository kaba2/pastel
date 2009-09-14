// Description: Find the nearest standard basis axis for a given vector.
// Documentation: common.txt

#ifndef PASTEL_NEAREST_MAIN_AXIS_H
#define PASTEL_NEAREST_MAIN_AXIS_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Finds the nearest standard basis axis for a given vector.
	template <int N, typename Real>
	integer nearestMainAxis(
		const Vector<Real, N>& that);

}

#include "pastel/geometry/nearest_main_axis.hpp"

#endif
