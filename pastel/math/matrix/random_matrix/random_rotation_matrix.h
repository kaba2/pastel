// Description: Uniform random rotation matrix

#ifndef PASTELMATH_RANDOM_ROTATION_MATRIX_H
#define PASTELMATH_RANDOM_ROTATION_MATRIX_H

#include "pastel/math/matrix/matrix.h"
#include "pastel/math/matrix/random_matrix/random_orthogonal_matrix.h"

#include <armadillo>

namespace Pastel
{

	//! Returns a uniformly distributed random rotation matrix.
	/*!
	Preconditions:
	n >= 0
	*/
	template <typename Real>
	arma::Mat<Real> randomRotation(integer n)
	{
		ENSURE_OP(n, >=, 0);
		return randomOrthogonal<Real>(n, 1);
	}

}

#endif
