#ifndef PASTEL_SIMPLEX_NOISE_H
#define PASTEL_SIMPLEX_NOISE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Simplex noise
	template <typename Real>
	Real simplexNoise(const PASTEL_NO_DEDUCTION(Real)& pos);

	//! Simplex noise
	template <typename Real, int N>
	Real simplexNoise(const Vector<Real, N>& pos);

}

#include "pastel/gfx/simplex_noise.hpp"

#endif
