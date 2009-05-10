#ifndef PASTELGEOMETRY_ELLIPSOID_H
#define PASTELGEOMETRY_ELLIPSOID_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Computes quadratic form coefficients for an ellipse.
	/*!
	An origin-centered ellipsoid Q is be given by:
	Q = {p | f(p) = 1}
	where
	f(p) = p S p^T
	p is in R^(1 x n)
	S is in R^(n x n) and symmetric positive semi-definite

	This function returns a matrix S such that
	the set Q is an image of the unit circle under
	the given linear transformation.
	*/

	template <int N, typename Real>
	Matrix<N, N, Real> ellipsoidQuadraticForm(
		const Matrix<N, N, Real>& basis);

	//! Returns a minimum volume aligned box bounding the given ellipse.

	template <int N, typename Real>
	AlignedBox<N, Real> ellipsoidBoundingAlignedBox(
		const Matrix<N, N, Real>& quadraticForm);

}

#include "pastel/geometry/ellipsoid.hpp"

#endif
