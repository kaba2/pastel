/*!
\file
\brief A function for computing the closest point on a plane w.r.t a point.
*/

#ifndef PASTEL_CLOSEST_PLANE_POINT_H
#define PASTEL_CLOSEST_PLANE_POINT_H

#include "pastel/geometry/plane.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	//! Computes the parameter of the closest point on a plane w.r.t a point.
	/*!
	If the returned value is t, then the closest point C is:
	C = point + t * plane.normal()
	*/

	template <int N, typename Real>
	Real closest(
		const Plane<N, Real>& plane,
		const Point<N, Real>& point);

}

#include "pastel/geometry/closest_plane_point.hpp"


#endif
