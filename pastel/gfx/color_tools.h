// Description: Algorithms for colors

#ifndef PASTELGFX_COLOR_TOOLS_H
#define PASTELGFX_COLOR_TOOLS_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"
#include "pastel/gfx/coloradapter.h"

namespace Pastel
{

	//! Generates a random rgb color.

	Color randomRgbColor();

	//! Returns the weights of the rgb-to-luma conversion.

	Color lumaWeights();

	//! Computes the luma of an rgb color.
	/*!
	The luma is computed from non-linear sRGB values
	and is a crude approximation to lightness. An ideal
	conversion would convert to CIELab, zero the ab components
	and convert back to sRGB. This is quite costly an
	operation and for many applications luma does good enough.
	*/
	real32 luma(const Color& rgb);

	//! Sets all colors out of the [0, 1] range to black.
	Color fitColorBlack(const Color& rgb);

	//! Fits an rgb color to [0, 1]^3 range while preserving hue.
	/*!
	Components less than zero are set to zero.
	If a color has a component greater than 1, then
	the color is normalized by max(rgb). This operation
	maps the color to the legal range by decreasing intensity
	while preserving hue.
	If no such cases apply, the color is returned as it is.
	*/
	Color fitColor(const Color& rgb);

	//! Sets negative components to zero.
	Color fitNegativeColor(const Color& rgb);

	class PASTELGFX Luma_Color_Adapter
	{
	public:
		typedef Color Logical;
		typedef real32 Physical;

		Logical convert(const Physical& physical) const
		{
			return Color(physical);
		}

		Physical revert(const Logical& logical) const
		{
			return luma(logical);
		}
	};

	typedef ReverseAdapter<Luma_Color_Adapter> Color_Luma_Adapter;

}

#include "pastel/gfx/color_tools.hpp"

#endif
