#ifndef PASTELGFX_BRANCHINGNUMBER_H
#define PASTELGFX_BRANCHINGNUMBER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/lineararray.h"

#include "pastel/gfx/gfxlibrary.h"

namespace Pastel
{

	//! Computes the branching number of each pixel.
	/*!
	The branching number of a pixel is the
	number of 4-connected 1-components in its
	8-neighbourhood (the pixel itself does not
	take part of any component). 

	Example:
	0 1 0
	1 x 1
	1 0 1
	The branching number of x is three.

	Time complexity: linear
	Exception safety: strong
	*/
	PASTELGFX void branchingNumber(
		const LinearArray<2, bool>& image,
		LinearArray<2, int32>& numberImage);

	PASTELGFX integer branchingNumber(
		const LinearArray<2, bool>& image,
		integer x, integer y);

	PASTELGFX uint32 branchingMask(
		const LinearArray<2, bool>& image,
		integer x, integer y);

}

#endif
