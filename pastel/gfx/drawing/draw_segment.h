// Description: Drawing a line segment
// Documentation: drawing.txt

#ifndef PASTELGFX_DRAW_SEGMENT_H
#define PASTELGFX_DRAW_SEGMENT_H

#include "pastel/geometry/shape/segment.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view/view.h"

namespace Pastel
{

	//! Draws a line segment.

	template <typename Type, typename Image_View>
	void drawSegmentOld(
		const Segment2& segment,
		const NoDeduction<Type>& color,
		const View<2, Type, Image_View>& image);

	//! Draws a line segment.

	template <typename Type, typename Image_View>
	void drawSegment(
		const Segment2& segment,
		const NoDeduction<Type>& color,
		const View<2, Type, Image_View>& image);

}

#include "pastel/gfx/drawing/draw_segment.hpp"

#endif
