// Description: Uniform random rotation matrix

#ifndef PASTELMATH_RANDOM_ROTATION_MATRIX_H
#define PASTELMATH_RANDOM_ROTATION_MATRIX_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Generates a uniformly distributed random rotation matrix.
	template <typename Real>
	void setRandomRotation(
		Matrix<Real>& result);

	//! Returns a uniformly distributed random rotation matrix.
	/*!
	Preconditions:
	n >= 0
	*/
	template <typename Real>
	Matrix<Real> randomRotationMatrix(
		integer n);

}

#include "pastel/math/random_rotation_matrix.hpp"

#endif
