#ifndef PASTELMATH_RANDOM_MATRIX_HPP
#define PASTELMATH_RANDOM_MATRIX_HPP

#include "pastel/math/matrix/random_matrix/random_matrix.h"

#include "pastel/sys/random/random_continuous.h"

namespace Pastel
{

	//! Returns a random (m x n)-matrix with elements from [-1, 1].
	/*!
	Preconditions:
	m >= 0
	n >= 0
	*/
	template <typename Real, int M = Dynamic, int N = Dynamic>
	Matrix<Real, M, N> randomMatrix(integer m = M, integer n = N)
	{
		return generateMatrix<Real, M, N>([](int i, int j) {return random<Real>();}, m, n);
	}

	//! Returns a random (m x n)-matrix with standard-normally-distributed elements.
	/*!
	Preconditions:
	m >= 0
	n >= 0
	*/
	template <typename Real, int M = Dynamic, int N = Dynamic>
	Matrix<Real, M, N> randomGaussianMatrix(integer m = M, integer n = N)
	{
		return generateMatrix<Real, M, N>([](int i, int j) {return randomGaussian<Real>();}, m, n);
	}

}

#endif
