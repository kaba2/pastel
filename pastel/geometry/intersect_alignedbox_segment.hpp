#ifndef PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_SEGMENT_HPP
#define PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_SEGMENT_HPP

#include "pastel/geometry/intersect_alignedbox_segment.h"

#include "pastel/geometry/intersect_line_alignedbox.h"

#include "pastel/sys/vector_tools.h"

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
		const Real tMax = std::min(tRange[1], Real(1));

		result.set(
			segment.at(tMin), segment.at(tMax));

		return true;
	}

}

#endif
