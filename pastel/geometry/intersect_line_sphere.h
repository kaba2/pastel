// Description: The intersection between a line and a sphere

#ifndef PASTEL_INTERSECT_LINE_SPHERE_H
#define PASTEL_INTERSECT_LINE_SPHERE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"
#include "pastel/geometry/sphere.h"
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

	template <int N, typename Real>
		bool intersect(
			const Line<Real, N>& line,
			const Sphere<Real, N>& sphere,
			Vector<Real, 2>& hitList);

}

#include "pastel/geometry/intersect_line_sphere.hpp"

#endif
