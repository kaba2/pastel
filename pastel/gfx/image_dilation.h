// Description: Dilation of a binary image
// Documentation: image_morphology.txt

#ifndef PASTEL_IMAGE_DILATION_H
#define PASTEL_IMAGE_DILATION_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"

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

#include "pastel/gfx/image_dilation.hpp"

#endif
