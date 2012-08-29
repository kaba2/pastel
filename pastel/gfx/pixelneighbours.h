// Description: Counting and examination of pixel neighbors
// Documentation: branching_number.txt

#ifndef PASTELGFX_PIXELNEIGHBOURS_H
#define PASTELGFX_PIXELNEIGHBOURS_H

#include "pastel/gfx/gfxlibrary.h"

#include "pastel/sys/array.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	PASTELGFX integer pixelNeighbours(
		const Array<bool, 2>& image,
		integer x, integer y);

	PASTELGFX uint32 pixelNeighbourMask(
		const Array<bool, 2>& image,
		integer x, integer y);

}

#endif
