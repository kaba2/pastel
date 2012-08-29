// Description: Algorithms for ellipsoids

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
	the set Q is an image of the unit sphere under
	the given linear transformation.
	*/

	template <typename Real, int N>
	Matrix<Real, N, N> ellipsoidQuadraticForm(
		const Matrix<Real, N, N>& basis);

	//! Returns a minimum volume aligned box bounding the given ellipse.

	template <typename Real, int N>
	AlignedBox<Real, N> ellipsoidBoundingAlignedBox(
		const Matrix<Real, N, N>& quadraticForm);

}

#include "pastel/geometry/ellipsoid.hpp"

#endif
