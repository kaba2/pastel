// Description: Area of a polygon
// Documentation: area.txt

#ifndef PASTEL_POLYGON_AREA_H
#define PASTEL_POLYGON_AREA_H

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

#include "pastel/geometry/polygon_area.hpp"

#endif
