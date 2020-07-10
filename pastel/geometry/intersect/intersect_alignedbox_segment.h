// Description: Intersection between an aligned box and a line segment

#ifndef PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_SEGMENT_H
#define PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_SEGMENT_H

#include "pastel/geometry/shape/segment.h"
#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/intersect/intersect_line_alignedbox.h"
#include "pastel/sys/vector/vector.h"

namespace Pastel
{

	template <typename Real, int N>
	bool intersect(
		const AlignedBox<Real, N>& alignedBox,
		const Segment<Real, N>& segment,
		Segment<Real, N>& result)
	{
		Line<Real, N> line(
			segment.start(),
			segment.end() - segment.start());

		Vector<Real, 2> tRange;
		if (!intersect(line, alignedBox, tRange))
		{
			return false;
		}
		if (tRange[0] > 1 ||
			tRange[1] < 0)
		{
			return false;
		}

		Real tMin = std::max(tRange[0], Real(0));
		Real tMax = std::min(tRange[1], Real(1));

		result.set(
			segment.at(tMin), segment.at(tMax));

		return true;
	}

}

#endif
