// Description: Counting and examination of pixel neighbors
// Documentation: branching_number.txt

#ifndef PASTELGFX_PIXELNEIGHBOURS_H
#define PASTELGFX_PIXELNEIGHBOURS_H

#include "pastel/sys/array.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	integer pixelNeighbours(
		const Array<bool, 2>& image,
		integer x, integer y);

	uint32 pixelNeighbourMask(
		const Array<bool, 2>& image,
		integer x, integer y);

}

#include "pastel/gfx/image_processing/pixelneighbours.hpp"

#endif
