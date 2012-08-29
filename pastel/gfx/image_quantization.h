// Description: Image quantization

#ifndef PASTELGFX_IMAGE_QUANTIZATION_H
#define PASTELGFX_IMAGE_QUANTIZATION_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	//! Quantizes using rounding to nearest level.

	template <typename Vector>
	void quantizeRounding(
		Array<Vector, 2>& image,
		const PASTEL_NO_DEDUCTION(Vector)& interval);

	//! Quantizes using Floyd-Steinberg error diffusion.

	template <typename Vector>
	void quantizeErrorDiffusion(
		Array<Vector, 2>& image,
		const PASTEL_NO_DEDUCTION(Vector)& interval);

}

#include "pastel/gfx/image_quantization.hpp"

#endif
