#ifndef PASTELGFX_PIXELNEIGHBOURS_H
#define PASTELGFX_PIXELNEIGHBOURS_H

#include "pastel/gfx/gfxlibrary.h"

#include "pastel/sys/lineararray.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	PASTELGFX integer pixelNeighbours(
		const LinearArray<2, bool>& image,
		integer x, integer y);

	PASTELGFX uint32 pixelNeighbourMask(
		const LinearArray<2, bool>& image,
		integer x, integer y);

}

#endif
