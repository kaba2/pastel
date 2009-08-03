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
		const Array<ImageType, 2>& image,
		const Array<bool, 2>& reconstructArea,
		const Array<FilterType, 2>& filter,
		Array<ResultType, 2>& result);

}

#include "pastel/dsp/reconstructimage.hpp"

#endif
