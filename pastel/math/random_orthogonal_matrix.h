// Description: Uniform random orthogonal matrix

#ifndef PASTELMATH_RANDOM_ORTHOGONAL_MATRIX_H
#define PASTELMATH_RANDOM_ORTHOGONAL_MATRIX_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Generates a uniformly distributed random orthogonal matrix.
	template <typename Real>
	void setRandomOrthogonal(
		Matrix<Real>& result);

	//! Returns a uniformly distributed random orthogonal matrix.
	template <typename Real>
	Matrix<Real> randomOrthogonalMatrix(
		integer n);

}

#include "pastel/math/random_orthogonal_matrix.hpp"

#endif
