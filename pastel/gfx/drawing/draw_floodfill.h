// Description: Flood-filling
// Documentation: drawing.txt

#ifndef PASTELGFX_DRAW_FLOODFILL_H
#define PASTELGFX_DRAW_FLOODFILL_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view/view.h"

namespace Pastel
{

	//! Changes the color of a connected region.
	/*!
	If xStart or yStart is out of range, nothing
	will happen. Otherwise the connected component
	that (xStart, yStart) is part of is
	recolored with the given color.
	*/
	template <typename Type, typename Image_View>
	void floodFill(
		integer xStart, integer yStart,
		const NoDeduction<Type>& color,
		const View<2, Type, Image_View>& image);

}

#include "pastel/gfx/drawing/draw_floodfill.hpp"

#endif
