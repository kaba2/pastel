// Description: Inverse affine transformation
// Documentation: affine_algorithms.txt

#ifndef PASTELMATH_AFFINE_INVERSE_H
#define PASTELMATH_AFFINE_INVERSE_H

#include "pastel/math/affine_transformation.h"

namespace Pastel
{

	//! Returns the inverse of an affine transformation.
	template <typename Real>
	AffineTransformation<Real> inverse(
		const AffineTransformation<Real>& that);

	//! Returns the inverse of an orthogonal affine transformation.
	template <typename Real>
	AffineTransformation<Real> orthogonalInverse(
		const AffineTransformation<Real>& that);

}

#include "pastel/math/affine_inverse.hpp"

#endif
