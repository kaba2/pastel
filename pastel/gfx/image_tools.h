#ifndef PASTELGFX_IMAGETOOLS_H
#define PASTELGFX_IMAGETOOLS_H

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

	typedef Color_Integer_Adapter<uint32, 8, 8, 8> Rgb888Adapter;

	template <int N>
	ConstView<N, Color, ConstAdaptedView<N, Rgb888Adapter, ConstArrayView<N, Array<N, uint32> > > >
		constRgb888View(const Array<N, uint32>& image);

	template <int N>
	View<N, Color, AdaptedView<N, Rgb888Adapter, ArrayView<N, Array<N, uint32> > > >
		rgb888View(Array<N, uint32>& image);

	PASTELGFX void indexedImageToColorImage(
		const Array<2, uint8>& indexedImage,
		const std::vector<Color>& palette,
		Array<2, Color>& colorImage);

	template <int N, typename Image_View>
	void scaleGrayscaleImage(
		const View<N, real32, Image_View>& image);

}

#include "pastel/gfx/image_tools.hpp"

#endif
