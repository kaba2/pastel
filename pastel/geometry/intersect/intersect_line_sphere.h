// Description: Intersection between a line and a sphere

#ifndef PASTELGEOMETRY_INTERSECT_LINE_SPHERE_H
#define PASTELGEOMETRY_INTERSECT_LINE_SPHERE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/shapes/line.h"
#include "pastel/geometry/shapes/sphere.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Finds the intersection between a line and a sphere.
	/*!
	Post conditions:
	If true is returned, hitList[0] <= hitList[1].

	If the line intersects the sphere, there will always be
	two entries in the hitlist. If the hit is tangential,
	the entries will be the same.
	*/

	template <typename Real, int N>
		bool intersect(
			const Line<Real, N>& line,
			const Sphere<Real, N>& sphere,
			Vector<Real, 2>& hitList);

}

#include "pastel/geometry/intersect/intersect_line_sphere.hpp"

#endif
