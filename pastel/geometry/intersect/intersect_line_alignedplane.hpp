#ifndef PASTELGEOMETRY_INTERSECT_LINE_ALIGNEDPLANE_HPP
#define PASTELGEOMETRY_INTERSECT_LINE_ALIGNEDPLANE_HPP

#include "pastel/geometry/intersect/intersect_line_alignedplane.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/shape/line.h"
#include "pastel/geometry/shape/alignedplane.h"

namespace Pastel
{

	template <typename Real, integer N>
	bool intersect(const Line<Real, N>& line,
		const AlignedPlane<Real, N>& alignedPlane, Real& t)
	{
		Real result(
			alignedPlane.position() - line.position()[alignedPlane.axis()]);
		Real divider(
			line.direction()[alignedPlane.axis()]);

		// EPSILON
		if (divider == 0)
		{
			// EPSILON
			if (result == 0)
			{
				t = 0;
				// Parallel, on the plane
				return true;
			}

			// Parallel, not on the plane.
			return false;
		}

		t = result / divider;

		return true;
	}

}

#endif
