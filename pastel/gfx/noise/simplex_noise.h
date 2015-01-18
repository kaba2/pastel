// Description: Perlin's simplex noise

#ifndef PASTELGFX_SIMPLEX_NOISE_H
#define PASTELGFX_SIMPLEX_NOISE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Simplex noise
	template <typename Real>
	Real simplexNoise(const NoDeduction<Real>& pos);

	//! Simplex noise
	template <typename Real, int N>
	Real simplexNoise(const Vector<Real, N>& pos);

}

#include "pastel/gfx/noise/simplex_noise.hpp"

#endif
