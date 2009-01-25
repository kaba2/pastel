/*!
\file
\brief A function for segmentation of a binary image into boxes.
*/

#ifndef PASTELGFX_BOXLABEL_H
#define PASTELGFX_BOXLABEL_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/lineararray.h"

#include "pastel/gfx/gfxlibrary.h"

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

    PASTELGFX integer boxLabel(
        const LinearArray<2, bool>& image,
        integer maxWidth,
        integer maxHeight,
        real maxRatio,
        LinearArray<2, int32>& labelImage);

}

#endif
