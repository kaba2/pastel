// Description: Bounding aligned box of a box

#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_BOX_H
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_BOX_H

#include "pastel/geometry/shapes/alignedbox.h"
#include "pastel/geometry/shapes/box.h"

namespace Pastel
{

	//! Bounding aligned box of an oriented box.

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Box<Real, N>& box);

}

#include "pastel/geometry/bounding/bounding_alignedbox_box.hpp"

#endif