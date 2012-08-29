// Description: LMS color space
// Detail: Long-Medium-Short wavelength
// Documentation: color_space.txt

#ifndef PASTELGFX_COLOR_LMS_H
#define PASTELGFX_COLOR_LMS_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Converts XYZ color LMS color.
	PASTELGFX Color xyzToLms(const Color& xyz);

	//! Converts LMS color XYZ color.
	PASTELGFX Color lmsToXyz(const Color& lms);

	//! Return the transformation from XYZ to LMS.
	PASTELGFX Matrix<real32, 3, 3> xyzToLmsTransform();

	//! Returns the transformation from LMS to XYZ.
	PASTELGFX Matrix<real32, 3, 3> lmsToXyzTransform();

}

#endif
