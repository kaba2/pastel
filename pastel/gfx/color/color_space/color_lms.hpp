#ifndef PASTELGFX_COLOR_LMS_HPP
#define PASTELGFX_COLOR_LMS_HPP

#include "pastel/gfx/color/color_space/color_lms.h"

#include "pastel/math/matrix/matrix.h"
#include "pastel/math/matrix/matrix_inverse.h"

namespace Pastel
{

	//! Return the transformation from XYZ to LMS.
	inline Matrix<real32, 3, 3> xyzToLmsTransform()
	{
		// This is the chromatic adaptation matrix
		// from the CIECAM02 model.
		static const Matrix<real32, 3, 3> Conversion = 
			matrix3x3<real32>(
			 0.7328, 0.4296, -0.1624,
		    -0.7036, 1.6975,  0.0061,
			 0.0030, 0.0136,  0.9834);

		return Conversion;
	}

	//! Returns the transformation from LMS to XYZ.
	inline Matrix<real32, 3, 3> lmsToXyzTransform()
	{
		static const Matrix<real32, 3, 3> Conversion =
			inverse(xyzToLmsTransform());

		return Conversion;
	}

	//! Converts XYZ color LMS color.
	inline Color xyzToLms(const Color& xyz)
	{
		return xyzToLmsTransform() * xyz;
	}

	//! Converts LMS color XYZ color.
	inline Color lmsToXyz(const Color& lms)
	{
		return lmsToXyzTransform() * lms;
	}

}

#endif
