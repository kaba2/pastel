// Description: Uniform random rotation matrix

#ifndef PASTELMATH_RANDOM_ROTATION_MATRIX_H
#define PASTELMATH_RANDOM_ROTATION_MATRIX_H

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Returns a uniformly distributed random rotation matrix.
	/*!
	Preconditions:
	n >= 0
	*/
	template <typename Real>
	Matrix<Real> randomRotation(integer n);

}

#include "pastel/math/matrix/random_matrix/random_rotation_matrix.hpp"

#endif
