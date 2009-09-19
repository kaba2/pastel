#include "pastel/gfx/color_lms.h"

#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	PASTELGFX Matrix<real32, 3, 3> xyzToLmsTransform()
	{
		static const Matrix<real32, 3, 3> Conversion(
			 0.7328, -0.7036, 0.0030,
			 0.4296,  1.6975, 0.0136,
			-0.1624,  0.0061, 0.9834);

		return Conversion;
	}

	PASTELGFX Matrix<real32, 3, 3> lmsToXyzTransform()
	{
		static const Matrix<real32, 3, 3> Conversion(
			inverse(xyzToLmsTransform()));

		return Conversion;
	}

	PASTELGFX Color xyzToLms(const Color& xyz)
	{
		return xyz * xyzToLmsTransform();
	}

	PASTELGFX Color lmsToXyz(const Color& lms)
	{
		return lms * lmsToXyzTransform();
	}

}
