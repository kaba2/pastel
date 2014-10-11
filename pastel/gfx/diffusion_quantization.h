// Description: Floyd-Steinberg error-diffusion quantization
// Documentation: image_quantization.txt

#ifndef PASTELGFX_DIFFUSION_QUANTIZATION_H
#define PASTELGFX_DIFFUSION_QUANTIZATION_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	//! Quantizes using Floyd-Steinberg error-diffusion.
	template <typename Vector>
	void quantizeErrorDiffusion(
		Array<Vector, 2>& image,
		const NoDeduction<Vector>& interval);

}

#include "pastel/gfx/diffusion_quantization.hpp"

#endif
