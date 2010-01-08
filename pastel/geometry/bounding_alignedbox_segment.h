// Description: Bounding aligned box of a line segment

#ifndef PASTEL_BOUNDING_ALIGNEDBOX_SEGMENT_H
#define PASTEL_BOUNDING_ALIGNEDBOX_SEGMENT_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/segment.h"

namespace Pastel
{

	//! Bounding aligned box of a line segment.

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Segment<Real, N>& segment);

}

#include "pastel/geometry/bounding_alignedbox_segment.hpp"

#endif
