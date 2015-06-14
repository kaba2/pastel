#ifndef PASTELGEOMETRY_INTERSECT_LINE_PLANE_HPP
#define PASTELGEOMETRY_INTERSECT_LINE_PLANE_HPP

#include "pastel/geometry/intersect/intersect_line_plane.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/shape/line.h"
#include "pastel/geometry/shape/plane.h"

namespace Pastel
{

	template <typename Real, integer N>
		bool intersect(
			const Line<Real, N>& line,
			const Plane<Real, N>& plane,
			Real& t)
	{
		// Let
		// P = line point
		// D = line direction
		// S = plane point
		// N = plane normal
		//
		// R(t) = P + tD
		// dot(N, S - R(t)) = 0
		// =>
		// dot(N, S - P) - t * dot(N, D) = 0
		// =>
		// t = dot(N, S - P) / dot(N, D)
		//
		// If dot(N, D) == 0, then
		// the line and plane are parallel.

		Real n = dot(line.direction(), plane.normal());
		Real m = dot(plane.normal(), plane.position() - line.position());

		// EPSILON
		if (n == 0)
		{
			// EPSILON
			if (m == 0)
			{
				t = 0;
				return true;
			}
			return false;
		}

		t = m / n;

		return true;
	}

}

#endif
