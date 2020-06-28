#ifndef PASTELMATH_AFFINE_SCALING_HPP
#define PASTELMATH_AFFINE_SCALING_HPP

#include "pastel/math/affine/affine_scaling.h"

namespace Pastel
{

	//! Returns a scaling affine transformation.
	template <typename Real, int N>
	AffineTransformation<Real, N, N> affineScaling(
		const Vector<Real, N>& scaling)
	{
		AffineTransformation<Real, N, N> result(
			diagonalMatrix<Real, N>(scaling));

		return result;
	}

}

#endif
