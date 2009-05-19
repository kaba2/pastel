#ifndef PASTEL_INTERSECT_LINE_PLANE_HPP
#define PASTEL_INTERSECT_LINE_PLANE_HPP

#include "pastel/geometry/intersect_line_plane.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	template <int N, typename Real>
		bool intersect(
			const Line<N, Real>& line,
			const Plane<N, Real>& plane,
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

		const Real denominator = dot(line.direction(), plane.normal());
		const Real numerator = dot(plane.normal(), plane.position() - line.position());

		// EPSILON
		if (denominator == 0)
		{
			// EPSILON
			if (numerator == 0)
			{
				t = 0;
				return true;
			}
			return false;
		}

		t = numerator / denominator;

		return true;
	}

}

#endif
