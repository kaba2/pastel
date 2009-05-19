#ifndef PASTEL_BOUNDING_BOX_H
#define PASTEL_BOUNDING_BOX_H

#include "pastel/geometry/box.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
	Box<N, Real> boundingBox(const AlignedBox<N, Real>& alignedBox);

}

#include "pastel/geometry/bounding_box.hpp"

#endif
