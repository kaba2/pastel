// Description: Standard illuminants

#ifndef PASTELGFX_COLOR_ILLUMINANT_H
#define PASTELGFX_COLOR_ILLUMINANT_H

#include "pastel/gfx/color.h"

namespace Pastel
{

	// Standard illuminants (CIE 1964)

	//! Returns the XYZ coordinates of CIE illuminant E.
	Color xyzIlluminantE();

	//! Returns the XYZ coordinates of CIE illuminant D50.
	Color xyzIlluminantD50();

	//! Returns the XYZ coordinates of CIE illuminant D65.
	Color xyzIlluminantD65();

}

#include "pastel/gfx/color_illuminant.hpp"

#endif
