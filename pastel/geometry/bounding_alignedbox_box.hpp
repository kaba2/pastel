#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_BOX_HPP
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_BOX_HPP

#include "pastel/geometry/bounding_alignedbox_box.h"

namespace Pastel
{

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Box<Real, N>& box)
	{
		// An oriented box is an affine 
		// transformation of an aligned box.
		// You can find the derivation in
		// 'bounding_alignedbox_alignedbox.h'.
		
		const Vector<Real, N> radius =
			box.width() * abs(box.rotation());
		
		const AlignedBox<Real, N> result(
			box.position() - radius, box.position() + radius);

		return result;
	}

}

#endif
