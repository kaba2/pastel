// Description: CIE-Lab color space
// Detail: Roughly perceptually uniform w.r.t. Euclidean distance
// Documentation: color_space.txt

#ifndef PASTEL_COLOR_LAB_H
#define PASTEL_COLOR_LAB_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

namespace Pastel
{

	//! Converts XYZ color to CIELab color
	PASTELGFX Color xyzToLab(const Color& xyz);

	//! Converts CIELab color to XYZ color
	PASTELGFX Color labToXyz(const Color& lab);

}

#endif
