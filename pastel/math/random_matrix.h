// Description: An aggregate file for random matrices.

#ifndef PASTELMATH_RANDOM_MATRIX_H
#define PASTELMATH_RANDOM_MATRIX_H

#include "pastel/math/random_orthogonal_matrix.h"
#include "pastel/math/random_rotation_matrix.h"
#include "pastel/math/random_spd_matrix.h"

namespace Pastel
{

	//! Returns a random matrix with elements from [-1, 1].
	template <typename Real>
	void setRandomMatrix(
		Matrix<Real>& matrix);

}

#include "pastel/math/random_matrix.hpp"

#endif
