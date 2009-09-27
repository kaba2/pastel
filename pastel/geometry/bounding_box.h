// Description: Bounding boxes

#ifndef PASTEL_BOUNDING_BOX_H
#define PASTEL_BOUNDING_BOX_H

#include "pastel/geometry/box.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <typename Real, int N>
	Box<Real, N> boundingBox(const AlignedBox<Real, N>& alignedBox);

}

#include "pastel/geometry/bounding_box.hpp"

#endif
