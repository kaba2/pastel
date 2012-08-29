// Description: Conversion of a labeled image to an indexed image

#ifndef PASTELGFX_LABELTOIMAGE_H
#define PASTELGFX_LABELTOIMAGE_H

#include "pastel/sys/array.h"

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

#include <vector>

namespace Pastel
{

	//! Converts a label image to an indexed image for visualization.

	/*!
	Labels 0-255 will be mapped to the equal color indices.
	Labels greater than that will be mapped in a
	seemingly random manner to the range 0-255.
	*/

	PASTELGFX void labelToImage(
		const Array<int32, 2>& labelImage,
		Array<uint8, 2>& image);

}

#endif
