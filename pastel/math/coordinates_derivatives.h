// Description: Derivatives of coordinate conversions

#ifndef PASTELMATH_COORDINATES_DERIVATES_H
#define PASTELMATH_COORDINATES_DERIVATES_H

#include "pastel/math/matrix.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Derivative of the cartesianToSpherical function.
	template <typename Real, int N>
	Matrix<Real, N, N> cartesianToSphericalDerivative(
		const Vector<Real, N>& cartesian);

	//! Derivative of the sphericalToCartesian function.
	template <typename Real, int N>
	Matrix<Real, N, N> sphericalToCartesianDerivative(
		const Vector<Real, N>& cartesian);

	//! Partial derivatives of the sphericalToCartesian function.
	/*!
	Preconditions:
	allGreaterEqual(index, 0)

	Let
	f : R^n -> R^n : f(r, alpha_1, ..., alpha_{n - 1}) = (x_0, ..., x_{n - 1})
	f = sphericalToCartesian
	
	Using multi-index notation, this function computes
	df / d(index) = d^|index| / ((dx_0)^index[0] ... (dx_(n-1))^index[n - 1])
	where
	|index| = sum(index)
	*/
	template <typename Real, int N>
	Vector<Real, N> sphericalToCartesianDerivative(
		const Vector<Real, N>& spherical, 
		const Vector<integer, N>& index);

}

#include "pastel/math/coordinates_derivatives.hpp"

#endif
