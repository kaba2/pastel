// Description: Drawing a line segment

#ifndef PASTEL_DRAW_SEGMENT_H
#define PASTEL_DRAW_SEGMENT_H

#include "pastel/geometry/segment.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"

namespace Pastel
{

	//! Draws a line segment.

	template <typename Type, typename Image_View>
	void drawSegmentOld(
		const Segment2& segment,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image);

	//! Draws a line segment.

	template <typename Type, typename Image_View>
	void drawSegment(
		const Segment2& segment,
		const PASTEL_NO_DEDUCTION(Type)& color,
		const View<2, Type, Image_View>& image);

}

#include "pastel/gfx/draw_segment.hpp"

#endif
