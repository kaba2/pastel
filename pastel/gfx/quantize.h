// Description: Image quantization

#ifndef PASTEL_QUANTIZE_H
#define PASTEL_QUANTIZE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	//! Quantizes using rounding to nearest level.

	template <typename Real>
	void quantizeRounding(
		Array<2, Real>& image,
		const Real& interval);

	//! Quantizes using Floyd-Steinberg error diffusion.

	template <typename Real>
	void quantizeErrorDiffusion(
		Array<2, Real>& image,
		const Real& interval);

}

#include "pastel/gfx/quantize.hpp"

#endif
