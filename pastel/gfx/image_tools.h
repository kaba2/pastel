// Description: Some algorithms for images
// Documentation: common.txt

#ifndef PASTEL_IMAGETOOLS_H
#define PASTEL_IMAGETOOLS_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"
#include "pastel/gfx/coloradapter.h"

#include "pastel/sys/array.h"
#include "pastel/sys/view.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/adaptedview.h"

#include <vector>

namespace Pastel
{

	typedef Integer_To_Color<uint32, 8, 8, 8> Rgb888Adapter;

	template <int N>
	ConstView<N, Color, ConstAdaptedView<N, Rgb888Adapter, ConstArrayView<N, Array<uint32, N> > > >
		constRgb888View(const Array<uint32, N>& image);

	template <int N>
	View<N, Color, AdaptedView<N, Rgb888Adapter, ArrayView<N, Array<uint32, N> > > >
		rgb888View(Array<uint32, N>& image);

	PASTELGFX void indexedImageToColorImage(
		const Array<uint8, 2>& indexedImage,
		const std::vector<Color>& palette,
		Array<Color, 2>& colorImage);

	template <int N, typename Image_View>
	void scaleGrayscaleImage(
		const View<N, real32, Image_View>& image);

}

#include "pastel/gfx/image_tools.hpp"

#endif
