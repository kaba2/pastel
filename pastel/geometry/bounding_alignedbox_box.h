#ifndef PASTEL_BOUNDING_ALIGNEDBOX_BOX_H
#define PASTEL_BOUNDING_ALIGNEDBOX_BOX_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	//! Bounding aligned box of an oriented box.

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Box<Real, N>& box);

}

#include "pastel/geometry/bounding_alignedbox_box.hpp"

#endif
