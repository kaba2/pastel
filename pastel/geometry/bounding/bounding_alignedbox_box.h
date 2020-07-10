// Description: Bounding aligned box of a box

#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_BOX_H
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_BOX_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/box.h"

namespace Pastel
{

	//! Bounding aligned box of an oriented box.
	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Box<Real, N>& box)
	{
		// An oriented box is an affine 
		// transformation of an aligned box.
		// You can find the derivation in
		// 'bounding_alignedbox_alignedbox.h'.
		
		Vector<Real, N> radius =
			box.width() * abs(box.rotation());
		
		AlignedBox<Real, N> result(
			box.position() - radius, box.position() + radius);

		return result;
	}

}

#endif
