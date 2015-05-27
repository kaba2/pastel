#ifndef PASTELGFX_COLOR_LMS_HPP
#define PASTELGFX_COLOR_LMS_HPP

#include "pastel/gfx/color/color_space/color_lms.h"

#include "pastel/math/matrix/matrix_inverse.h"

namespace Pastel
{

	inline Matrix<real32> xyzToLmsTransform()
	{
		// This is the chromatic adaptation matrix
		// from the CIECAM02 model.
		static const Matrix<real32> Conversion = 
			matrix3x3<real32>(
			 0.7328, 0.4296, -0.1624,
		    -0.7036, 1.6975,  0.0061,
			 0.0030, 0.0136,  0.9834);

		return Conversion;
	}

	inline Matrix<real32> lmsToXyzTransform()
	{
		static const Matrix<real32> Conversion =
			inverse(xyzToLmsTransform());

		return Conversion;
	}

	inline Color xyzToLms(const Color& xyz)
	{
		return xyzToLmsTransform() * xyz;
	}

	inline Color lmsToXyz(const Color& lms)
	{
		return lmsToXyzTransform() * lms;
	}

}

#endif
