#ifndef PASTELMATH_MATRIX_EIGEN_HPP
#define PASTELMATH_MATRIX_EIGEN_HPP

#include "pastel/math/matrix_eigen.h"
#include "pastel/math/matrix_trace.h"
#include "pastel/math/matrix_determinant.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real>
	Vector<Real, 2> symmetricEigenValues(
		const Matrix<Real>& matrix)
	{
		// Let the matrix be
		//
		// [a b]
		// [c d]
		//
		// For 2 x 2 matrices we can solve the eigenvalues directly
		// from the characteristic equation:
		//
		// det [a - k     b] = 0
		//     [c     d - k]
		// =>
		// (a - k)(d - k) - bc = 0
		// =>
		// ad - k(a + d) + k^2 - bc = 0
		// =>
		// k^2 - k(a + d) + (ad - bc) = 0

		Vector<Real, 2> eigenValue;

		quadratic(
			1, -trace(matrix), determinant(matrix),
			eigenValue[0], eigenValue[1], true);

		return eigenValue;
	}

	template <typename Real>
	void symmetricEigenDecomposition(
		const Matrix<Real>& matrix,
		Matrix<Real>& eigenVector,
		Vector<Real, 2>& eigenValue)
	{
		eigenValue = symmetricEigenValues(matrix);

		// Because the eigenvalues are listed in ascending order,
		// the axis lengths are listed in descending order:
		// thus the eigenvalue corresponding to the longest axis
		// is in 'eigenValue[0]'.

		// If an eigenvalue k is given, then a corresponding
		// eigenvector is a [b, k - a] or [k - d, c].
		// We have to be careful for the case
		// in which both of these vectors are zero.
		// This happens precisely when the 'matrix' is
		// a multiple of the identity matrix.

		Vector<Real, 2> aCandidate(
			matrix(1, 0), eigenValue[0] - matrix(0, 0));

		if (dot(aCandidate) > square(0.001))
		{
			eigenVector[0] = aCandidate;
		}
		else
		{
			const Vector<Real, 2> bCandidate(
				eigenValue[0] - matrix(1, 1), matrix(0, 1));
			if (dot(bCandidate) > square(0.001))
			{
				eigenVector[0] = bCandidate;
			}
			else
			{
				eigenVector[0].set(1, 0);
			}
		}

		// Because S is symmetric and real, the eigenvectors
		// must be orthogonal to each other.
		eigenVector[1] = cross(eigenVector[0]);
	}

}

#endif
