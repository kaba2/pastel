// Description: Branching number computation
// Documentation: branching_number.txt

#ifndef PASTELGFX_BRANCHING_NUMBER_H
#define PASTELGFX_BRANCHING_NUMBER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/array.h"

namespace Pastel
{

	//! Computes the branching number of each pixel.
	/*!
	Time complexity: linear
	Exception safety: strong
	*/
	void branchingNumber(
		const Array<bool, 2>& image,
		Array<int32, 2>& numberImage);

	integer branchingNumber(
		const Array<bool, 2>& image,
		integer x, integer y);

	uint32 branchingMask(
		const Array<bool, 2>& image,
		integer x, integer y);

}

#include "pastel/gfx/image_processing/branching_number.hpp"

#endif
