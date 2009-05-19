/*!
\file
\brief A function for dilating a binary image with another image.
*/

#ifndef PASTEL_DILATE_H
#define PASTEL_DILATE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"
#include "pastel/sys/rectangle_tools.h"

namespace Pastel
{

	//! Or's an image into another image.

	template <
		typename Element_ConstView,
		typename Image_View>
		void logicOr(
		const ConstView<2, bool, Element_ConstView>& element,
		integer x,
		integer y,
		const View<2, bool, Image_View>& target);

	//! Dilates the given image with another image.

	template <
		typename Input_ConstView,
		typename Element_ConstView,
		typename Output_View>
		void dilate(
		const ConstView<2, bool, Input_ConstView>& image,
		const ConstView<2, bool, Element_ConstView>& element,
		integer xPivot,
		integer yPivot,
		const View<2, bool, Output_View>& output);

}

#include "pastel/gfx/dilate.hpp"

#endif
