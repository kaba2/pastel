/*!
\file
\brief A function for eroding a binary image with another image.
*/

#ifndef PASTELGFX_ERODE_H
#define PASTELGFX_ERODE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"
#include "pastel/sys/extendedconstview.h"

namespace Pastel
{

	//! Erodes the given image with another image.

	template <
		typename Input_ConstView,
		typename Element_ConstView,
		typename Output_View>
		void erode(
		const ExtendedConstView<2, bool, Input_ConstView>& input,
		const ConstView<2, bool, Element_ConstView>& element,
		const View<2, bool, Output_View>& output,
		integer xPivot,
		integer yPivot);

	//! Erodes the given image with another image.
	/*!
	This version assumes that the pivot point of the
	structuring element is at the center.
	*/

	template <
		typename Input_ConstView,
		typename Element_ConstView,
		typename Output_View>
		void erode(
		const ExtendedConstView<2, bool, Input_ConstView>& input,
		const ConstView<2, bool, Element_ConstView>& element,
		const View<2, bool, Output_View>& output);

}

#include "pastel/gfx/erode.hpp"

#endif
