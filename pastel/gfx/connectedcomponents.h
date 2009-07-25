// Description: Connected components of a binary image

#ifndef PASTEL_CONNECTEDCOMPONENTS_H
#define PASTEL_CONNECTEDCOMPONENTS_H

#include "pastel/sys/view.h"

namespace Pastel
{

	//! Finds the connected components of a binary image.

	template <
		typename Image_ConstView,
		typename Result_View
	>
	void findConnectedComponents(
		const ConstView<2, bool, Image_ConstView>& image,
		const View<2, uint32, Result_View>& result);

}

#include "pastel/gfx/connectedcomponents.hpp"

#endif
