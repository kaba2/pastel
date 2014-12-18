// Description: Area of a polygon
// Documentation: area.txt

#ifndef PASTELGEOMETRY_POLYGON_AREA_H
#define PASTELGEOMETRY_POLYGON_AREA_H

#include "pastel/geometry/shapes/polygon.h"

namespace Pastel
{

	//! Computes the signed area of a simple polygon.
	/*!
	Time complexity: linear
	*/

	template <typename Real>
	Real signedArea(const Polygon<Real, 2>& polygon);

}

#include "pastel/geometry/area/polygon_area.hpp"

#endif
