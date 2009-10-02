// Description: Perlin's classic noise function
// Documentation: noise.txt

#ifndef PASTEL_PERLIN_NOISE_H
#define PASTEL_PERLIN_NOISE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/ray/raylibrary.h"

namespace Pastel
{

	//! Perlin noise.
	/*!
	The returned values are in the range [0, 1].
	*/
	template <typename Real>
	Real noise(const PASTEL_NO_DEDUCTION(Real)& pos);

	//! Perlin noise
	/*!
	The returned values are in the range [0, 1].
	*/
	template <typename Real, int N>
	Real noise(const Vector<Real, N>& pos);

}

#include "pastel/gfx/perlin_noise.hpp"

#endif
