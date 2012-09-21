// Description: Inverse of a conformal affine transformation.
// Documentation: conformalaffine2d_algorithms.txt

#ifndef PASTELMATH_CONFORMALAFFINE2D_INVERSE_H
#define PASTELMATH_CONFORMALAFFINE2D_INVERSE_H

#include "pastel/math/conformalaffine2d.h"

namespace Pastel
{

	//! Returns the inverse of a conformal affine transformation.
	template <typename Real>
	ConformalAffine2D<Real> inverse(
		const ConformalAffine2D<Real>& that);

}

#include "pastel/math/conformalaffine2d_inverse.hpp"

#endif
