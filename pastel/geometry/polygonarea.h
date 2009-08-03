// Description: Tools for computing polygon area in 2D
// Documentation: area.txt

#ifndef PASTEL_POLYGONAREA_H
#define PASTEL_POLYGONAREA_H

#include "pastel/geometry/polygon.h"

namespace Pastel
{

	//! Computes the signed area of a simple polygon.
	/*!
	Time complexity: linear
	*/

	template <typename Real>
	Real signedArea(const Polygon<Real, 2>& polygon);

}

#include "pastel/geometry/polygonarea.hpp"

#endif
