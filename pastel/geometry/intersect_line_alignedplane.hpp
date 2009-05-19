#ifndef PASTEL_INTERSECT_LINE_ALIGNEDPLANE_HPP
#define PASTEL_INTERSECT_LINE_ALIGNEDPLANE_HPP

#include "pastel/geometry/intersect_line_alignedplane.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"
#include "pastel/geometry/alignedplane.h"

namespace Pastel
{

	template <int N, typename Real>
	bool intersect(const Line<N, Real>& line,
		const AlignedPlane<N, Real>& alignedPlane, Real& t)
	{
		const Real result(
			alignedPlane.position() - line.position()[alignedPlane.axis()]);
		const Real divider(
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
