// Description: Intersection between two aligned boxes
// Documentation: alignedbox.txt

#ifndef PASTELSYS_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTELSYS_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/sys/alignedbox.h"

namespace Pastel
{

	//! Finds the intersection of two aligned boxes.
	/*!
	aBox, bBox:
	The boxes whose intersection to compute.

	result:
	A box which is contained in both 'aBox' and 'bBox'.
	Can be the same as 'aBox' or 'bBox'. 
	
	Returns:
	Whether the boxes intersect.
	*/
	template <typename Real, int N>
	bool intersect(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		AlignedBox<Real, N>& result);

}

#include "pastel/sys/intersect_alignedbox_alignedbox.hpp"

#endif
