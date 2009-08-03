// Description: Perlin's noise functions

#ifndef PASTEL_NOISE_H
#define PASTEL_NOISE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/point.h"
#include "pastel/ray/raylibrary.h"

namespace Pastel
{

	//! 1d Perlin noise.
	/*!
	The returned values are in the range [0, 1].
	*/

	template <typename Real>
	Real noise(const Real& pos);

	//! 2d Perlin noise.
	/*!
	The returned values are in the range [0, 1].
	*/

	template <typename Real>
	Real noise(const Point<Real, 2>& pos);

	//! 3d Perlin noise.
	/*!
	The returned values are in the range [0, 1].
	*/

	template <typename Real>
	Real noise(const Point<Real, 3>& pos);

}

#include "pastel/gfx/noise.hpp"

#endif
