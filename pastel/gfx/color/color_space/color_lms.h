// Description: LMS color space
// Detail: Long-Medium-Short wavelength
// Documentation: color_space.txt

#ifndef PASTELGFX_COLOR_LMS_H
#define PASTELGFX_COLOR_LMS_H

#include "pastel/gfx/color/color.h"

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Converts XYZ color LMS color.
	Color xyzToLms(const Color& xyz);

	//! Converts LMS color XYZ color.
	Color lmsToXyz(const Color& lms);

	//! Return the transformation from XYZ to LMS.
	Matrix<real32> xyzToLmsTransform();

	//! Returns the transformation from LMS to XYZ.
	Matrix<real32> lmsToXyzTransform();

}

#include "pastel/gfx/color/color_space/color_lms.hpp"

#endif
