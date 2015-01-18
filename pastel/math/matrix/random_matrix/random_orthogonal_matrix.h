// Description: Uniform random orthogonal matrix

#ifndef PASTELMATH_RANDOM_ORTHOGONAL_MATRIX_H
#define PASTELMATH_RANDOM_ORTHOGONAL_MATRIX_H

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Returns a uniformly distributed random orthogonal matrix.
	/*!
	Preconditions:
	n >= 0

	The returned matrix is Q in RR^{n x n} such that
	Q^T Q = I. If orientation != 0, then also 
	det(Q) = sign(orientation).

	n:
	The dimension of the matrix; Q in R^{n x n}.

	orientation:
	If zero, then det(Q) = +-1.
	If negative, then det(Q) = -1.
	If positive, then det(Q) = +1.
	*/
	template <typename Real>
	Matrix<Real> randomOrthogonal(
		integer n, integer orientation = 0);

}

#include "pastel/math/matrix/random_matrix/random_orthogonal_matrix.hpp"

#endif
