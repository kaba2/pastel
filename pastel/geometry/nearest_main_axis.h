// Description: Find the nearest standard basis axis for a given vector.
// Documentation: common.txt

#ifndef PASTELGEOMETRY_NEAREST_MAIN_AXIS_H
#define PASTELGEOMETRY_NEAREST_MAIN_AXIS_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Finds the nearest standard basis axis for a given vector.
	template <typename Real, int N>
	integer nearestMainAxis(
		const Vector<Real, N>& that)
	{
		// projectedLength = |<e_i, that> / |e_i|| = |that[i]|
		return maxIndex(abs(that));
	}

}

#endif
