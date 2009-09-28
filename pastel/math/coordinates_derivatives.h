// Description: Derivatives of coordinate conversions

#ifndef PASTEL_COORDINATES_DERIVATES_H
#define PASTEL_COORDINATES_DERIVATES_H

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

}

#include "pastel/math/coordinates_derivatives.hpp"

#endif
