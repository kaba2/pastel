// Description: Bounding boxes

#ifndef PASTELGEOMETRY_BOUNDING_BOX_H
#define PASTELGEOMETRY_BOUNDING_BOX_H

#include "pastel/geometry/box.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <typename Real, int N>
	Box<Real, N> boundingBox(const AlignedBox<Real, N>& alignedBox);

}

#include "pastel/geometry/bounding/bounding_box.hpp"

#endif
