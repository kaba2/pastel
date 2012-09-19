// Description: Matrix eigen-decomposition
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_EIGEN_H
#define PASTELMATH_MATRIX_EIGEN_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	template <typename Real>
	Vector<Real, 2> symmetricEigenValues(
		const Matrix<Real>& matrix);

	template <typename Real>
	void symmetricEigenDecomposition(
		const Matrix<Real>& matrix,
		Matrix<Real>& eigenVector,
		Vector<Real, 2>& eigenValue);

}

#include "pastel/math/matrix_eigen.hpp"

#endif
