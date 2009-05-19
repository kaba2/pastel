#ifndef PASTEL_INTERSECT_ALIGNEDBOX_SEGMENT_HPP
#define PASTEL_INTERSECT_ALIGNEDBOX_SEGMENT_HPP

#include "pastel/geometry/intersect_alignedbox_segment.h"

#include "pastel/geometry/intersect_line_alignedbox.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	bool intersect(
		const AlignedBox<N, Real>& alignedBox,
		const Segment<N, Real>& segment,
		Segment<N, Real>& result)
	{
		const Line<N, Real> line(
			segment.start(),
			segment.end() - segment.start());

		Vector<2, Real> tRange;
		if (!intersect(line, alignedBox, tRange))
		{
			return false;
		}
		if (tRange[0] > 1 ||
			tRange[1] < 0)
		{
			return false;
		}

		const Real tMin = std::max(tRange[0], Real(0));
		const Real tMax = std::min(tRange[1], Real(1));

		result.set(
			segment.at(tMin), segment.at(tMax));

		return true;
	}

}

#endif
