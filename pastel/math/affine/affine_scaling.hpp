#ifndef PASTELMATH_AFFINE_SCALING_HPP
#define PASTELMATH_AFFINE_SCALING_HPP

#include "pastel/math/affine/affine_scaling.h"

namespace Pastel
{

	template <typename Real, integer N>
	AffineTransformation<Real> affineScaling(
		const Vector<Real, N>& scaling)
	{
		AffineTransformation<Real> result(
			diagonalMatrix<Real>(scaling));

		return result;
	}

}

#endif
