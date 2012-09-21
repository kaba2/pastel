// Description: Scaling affine transformation
// Documentation: affine_algorithms.txt

#ifndef PASTELMATH_AFFINE_SCALING_H
#define PASTELMATH_AFFINE_SCALING_H

#include "pastel/math/affine_transformation.h"

namespace Pastel
{

	//! Returns a scaling affine transformation.
	template <typename Real, int N>
	AffineTransformation<Real> affineScaling(
		const Vector<Real, N>& scaling);

}

#include "pastel/math/affine_scaling.hpp"

#endif
