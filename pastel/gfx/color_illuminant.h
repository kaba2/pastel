// Description: Standard illuminants

#ifndef PASTELGFX_COLOR_ILLUMINANT_H
#define PASTELGFX_COLOR_ILLUMINANT_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

namespace Pastel
{

	// Standard illuminants (CIE 1964)

	//! Returns the XYZ coordinates of CIE illuminant E.
	PASTELGFX Color xyzIlluminantE();

	//! Returns the XYZ coordinates of CIE illuminant D50.
	PASTELGFX Color xyzIlluminantD50();

	//! Returns the XYZ coordinates of CIE illuminant D65.
	PASTELGFX Color xyzIlluminantD65();

}

#include "pastel/gfx/color_illuminant.hpp"

#endif
