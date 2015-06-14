// Description: Bounding aligned box of a line segment

#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_SEGMENT_H
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_SEGMENT_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/segment.h"

namespace Pastel
{

	//! Bounding aligned box of a line segment.

	template <typename Real, integer N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Segment<Real, N>& segment);

}

#include "pastel/geometry/bounding/bounding_alignedbox_segment.hpp"

#endif
