// Description: Perlin's classic noise function
// Documentation: noise.txt

#ifndef PASTEL_PERLIN_NOISE_H
#define PASTEL_PERLIN_NOISE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/ray/raylibrary.h"

namespace Pastel
{

	//! 1d Perlin noise.
	/*!
	The returned values are in the range [0, 1].
	*/

	template <typename Real>
	Real noise(const PASTEL_NO_DEDUCTION(Real)& pos);

	template <typename Real>
	Real noise(const Vector<Real, 1>& position);

	//! 2d Perlin noise.
	/*!
	The returned values are in the range [0, 1].
	*/

	template <typename Real>
	Real noise(const Vector<Real, 2>& pos);

	//! 3d Perlin noise.
	/*!
	The returned values are in the range [0, 1].
	*/

	template <typename Real>
	Real noise(const Vector<Real, 3>& pos);

}

#include "pastel/gfx/perlin_noise.hpp"

#endif
