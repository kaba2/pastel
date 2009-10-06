// Description: Perlin's classic noise
// Documentation: noise.txt

#ifndef PASTEL_PERLIN_NOISE_H
#define PASTEL_PERLIN_NOISE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Perlin noise
	/*!
	The returned values are in the range [0, 1].
	*/
	template <typename Real>
	Real perlinNoise(const PASTEL_NO_DEDUCTION(Real)& position);

	//! Perlin noise
	/*!
	The returned values are in the range [0, 1].
	*/
	template <typename Real, int N>
	Real perlinNoise(const Vector<Real, N>& position);

}

#include "pastel/gfx/perlin_noise.hpp"

#endif
