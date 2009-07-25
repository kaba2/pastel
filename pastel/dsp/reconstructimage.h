// Description: Reconstruction of missing pixels for an image

#ifndef PASTEL_RECONSTRUCTIMAGE_H
#define PASTEL_RECONSTRUCTIMAGE_H

#include "pastel/sys/ensure.h"
#include "pastel/sys/array.h"

namespace Pastel
{

	template <typename ImageType,
		typename FilterType,
		typename ResultType>
		void reconstructImage(
		const Array<2, ImageType>& image,
		const Array<2, bool>& reconstructArea,
		const Array<2, FilterType>& filter,
		Array<2, ResultType>& result);

}

#include "pastel/dsp/reconstructimage.hpp"

#endif
