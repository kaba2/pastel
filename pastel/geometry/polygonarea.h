#ifndef PASTELGEOMETRY_POLYGONAREA_H
#define PASTELGEOMETRY_POLYGONAREA_H

#include "pastel/geometry/polygon.h"

namespace Pastel
{

	//! Computes the signed area of a simple polygon.
	/*!
	Time complexity: linear
	*/

	template <typename Real>
	Real signedArea(const Polygon<2, Real>& polygon);

}

#include "pastel/geometry/polygonarea.hpp"

#endif
