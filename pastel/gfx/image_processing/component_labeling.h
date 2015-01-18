// Description: Connected components of a binary image
// Documentation: labeling.txt

#ifndef PASTELGFX_COMPONENT_LABELING_H
#define PASTELGFX_COMPONENT_LABELING_H

#include "pastel/sys/view/view.h"

namespace Pastel
{

	//! Finds the connected components of a binary image.

	template <
		typename Image_ConstView,
		typename Result_View
	>
	void labelConnectedComponents(
		const ConstView<2, bool, Image_ConstView>& image,
		const View<2, uint32, Result_View>& result);

}

#include "pastel/gfx/component_labeling.hpp"

#endif
