#ifndef PASTELGEOMETRY_DISTANCE_PLANE_POINT_HPP
#define PASTELGEOMETRY_DISTANCE_PLANE_POINT_HPP

#include "pastel/geometry/distance_plane_point.h"
#include "pastel/geometry/closest_plane_point.h"

#include "pastel/sys/mathfunctions.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(
		const Plane<N, Real>& plane,
		const Point<N, Real>& point)
	{
		// Let
		// D = a point on the plane
		// N = a unit normal of the plane
		// P = a point
		//
		// Let the closest point on the plane
		// w.r.t the point be C = P + tN.

		// Now
		// |C - P| = |t * N| = |t|

		const Real t = closest(plane, point);
		return t * t;
	}

}

#endif
