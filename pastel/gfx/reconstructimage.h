#ifndef PASTELGFX_RECONSTRUCTIMAGE_H
#define PASTELGFX_RECONSTRUCTIMAGE_H

#include "pastel/sys/ensure.h"
#include "pastel/sys/lineararray.h"

namespace Pastel
{

	template <typename ImageType,
		typename FilterType,
		typename ResultType>
		void reconstructImage(
		const LinearArray<2, ImageType>& image,
		const LinearArray<2, bool>& reconstructArea,
		const LinearArray<2, FilterType>& filter,
		LinearArray<2, ResultType>& result);

}

#include "pastel/gfx/reconstructimage.hpp"

#endif
