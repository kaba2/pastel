#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_SEGMENT_HPP
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_SEGMENT_HPP

#include "pastel/geometry/bounding_alignedbox_segment.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Segment<Real, N>& segment)
	{
		AlignedBox<Real, N> result(
			min(segment.start(), segment.end()),
			max(segment.start(), segment.end()));

		return result;
	}

}

#endif
