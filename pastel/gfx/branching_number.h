// Description: Branching number computation
// Documentation: branching_number.txt

#ifndef PASTEL_BRANCHING_NUMBER_H
#define PASTEL_BRANCHING_NUMBER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/array.h"

#include "pastel/gfx/gfxlibrary.h"

namespace Pastel
{

	//! Computes the branching number of each pixel.
	/*!
	Time complexity: linear
	Exception safety: strong
	*/
	PASTELGFX void branchingNumber(
		const Array<bool, 2>& image,
		Array<int32, 2>& numberImage);

	PASTELGFX integer branchingNumber(
		const Array<bool, 2>& image,
		integer x, integer y);

	PASTELGFX uint32 branchingMask(
		const Array<bool, 2>& image,
		integer x, integer y);

}

#endif
