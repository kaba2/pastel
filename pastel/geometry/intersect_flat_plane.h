/*!
\file
\brief A function for finding the intersection between a flat and a hyperplane.
*/

#ifndef PASTELGEOMETRY_INTERSECT_FLAT_PLANE_H
#define PASTELGEOMETRY_INTERSECT_FLAT_PLANE_H

#include "pastel/geometry/flat.h"
#include "pastel/geometry/plane.h"
#include "pastel/geometry/line.h"

namespace Pastel
{

	//! Finds the intersection between a 2-flat and a hyperplane.

	template <int N, typename Real>
	bool intersect(
		const Flat<N, Real, 2>& aPlane,
		const Plane<N, Real>& bPlane,
		Line<N, Real>& line);

}

#include "pastel/geometry/intersect_flat_plane.hpp"

#endif
