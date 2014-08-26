// Description: CIE-Lab color space
// Detail: Roughly perceptually uniform w.r.t. Euclidean distance
// Documentation: color_space.txt

#ifndef PASTELGFX_COLOR_LAB_H
#define PASTELGFX_COLOR_LAB_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

namespace Pastel
{

	//! Converts XYZ color to CIELab color
	Color xyzToLab(const Color& xyz);

	//! Converts CIELab color to XYZ color
	Color labToXyz(const Color& lab);

}

#include "pastel/gfx/color_lab.hpp"

#endif
