// Description: An aggregate file for random matrices.

#ifndef PASTELMATH_RANDOM_MATRIX_H
#define PASTELMATH_RANDOM_MATRIX_H

#include "pastel/math/random_orthogonal_matrix.h"
#include "pastel/math/random_rotation_matrix.h"
#include "pastel/math/random_spd_matrix.h"

namespace Pastel
{

	//! Returns a random (m x n)-matrix with elements from [-1, 1].
	/*!
	Preconditions:
	m >= 0
	n >= 0
	*/
	template <typename Real>
	Matrix<Real> randomMatrix(integer m, integer n);

}

#include "pastel/math/random_matrix.hpp"

#endif
