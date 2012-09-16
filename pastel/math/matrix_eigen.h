// Description: Matrix eigen-decomposition

#ifndef PASTELMATH_MATRIX_EIGEN_H
#define PASTELMATH_MATRIX_EIGEN_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	template <typename Real>
	Vector<Real, 2> symmetricEigenValues(
		const Matrix<Real, 2, 2>& matrix);

	template <typename Real>
	void symmetricEigenDecomposition(
		const Matrix<Real, 2, 2>& matrix,
		Matrix<Real, 2, 2>& eigenVector,
		Vector<Real, 2>& eigenValue);

}

#include "pastel/math/matrix_eigen.hpp"

#endif
