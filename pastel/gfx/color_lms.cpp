#include "pastel/gfx/color_lms.h"

#include "pastel/math/matrix_inverse.h"

namespace Pastel
{

	PASTELGFX Matrix<real32> xyzToLmsTransform()
	{
		// This is the chromatic adaptation matrix
		// from the CIECAM02 model.
		static PASTEL_CONSTEXPR Matrix<real32> Conversion = 
			matrix3x3<real32>(
			 0.7328, 0.4296, -0.1624,
		    -0.7036, 1.6975,  0.0061,
			 0.0030, 0.0136,  0.9834);

		return Conversion;
	}

	PASTELGFX Matrix<real32> lmsToXyzTransform()
	{
		static PASTEL_CONSTEXPR Matrix<real32> Conversion(
			inverse(xyzToLmsTransform()));

		return Conversion;
	}

	PASTELGFX Color xyzToLms(const Color& xyz)
	{
		return xyzToLmsTransform() * xyz;
	}

	PASTELGFX Color lmsToXyz(const Color& lms)
	{
		return lmsToXyzTransform() * lms;
	}

}
