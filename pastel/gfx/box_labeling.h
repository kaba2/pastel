// Description: Segmentation of a binary image into boxes
// Documentation: labeling.txt

#ifndef PASTELGFX_BOX_LABELING_H
#define PASTELGFX_BOX_LABELING_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/array.h"

namespace Pastel
{

	//! Segments a binary image into boxes.
	/*!
	The label 0 means empty, the boxes use labels >= 1.

	Parameters:
	image
		A binary image to label.
	maxWidth
		Maximum width of the segmentation boxes.
		Set this to image.width() or greater for no restrictions.
	maxHeight
		Maximum height of the segmentation boxes.
		Set this to image.height() or greater for no restrictions.
	maxRatio
		Maximum for max(boxWidth / boxHeight, boxHeight / boxWidth).
		This parameter can be used to prevent boxes that are too skinny.
		Set this to 1 or less for square boxes exclusively.
		Set this to infinity for no restrictions.

	Returns:
		The number of boxes labeled.

	Preconditions:
		maxWidth > 0
		maxHeight > 0
		image.width() <= labelImage.width()
		image.height() <= labelImage.height()

	Time complexity:
		Linear

	Exception safety:
		nothrow
	*/

	integer labelWithBoxes(
		const Array<bool, 2>& image,
		integer maxWidth,
		integer maxHeight,
		real maxRatio,
		Array<int32, 2>& labelImage);

}

#include "pastel/gfx/box_labeling.hpp"

#endif
