// Description: Rounding image quantization
// Documentation: image_quantization.txt

#ifndef PASTELGFX_ROUNDING_QUANTIZATION_H
#define PASTELGFX_ROUNDING_QUANTIZATION_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	//! Quantizes using rounding to nearest level.

	template <typename Vector>
	void quantizeRounding(
		Array<Vector, 2>& image,
		const NoDeduction<Vector>& interval);

}

#include "pastel/gfx/rounding_quantization.hpp"

#endif
