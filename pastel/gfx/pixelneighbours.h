#ifndef PASTEL_PIXELNEIGHBOURS_H
#define PASTEL_PIXELNEIGHBOURS_H

#include "pastel/gfx/gfxlibrary.h"

#include "pastel/sys/array.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	PASTELGFX integer pixelNeighbours(
		const Array<2, bool>& image,
		integer x, integer y);

	PASTELGFX uint32 pixelNeighbourMask(
		const Array<2, bool>& image,
		integer x, integer y);

}

#endif
