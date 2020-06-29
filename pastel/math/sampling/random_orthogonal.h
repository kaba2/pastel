// Description: Uniform random rotation
// Documentation: random_rotation_matrix.txt

#ifndef PASTELMATH_RANDOM_ORTHOGONAL_H
#define PASTELMATH_RANDOM_ORTHOGONAL_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Returns a uniform random orthogonal matrix.
	/*!
	Preconditions:
	n >= 0

	Input
	-----

	n (integer):
	The dimension of the returned matrix.

	Optional arguments
	------------------

	orientation (integer : 0):
	Specifies constraints for the determinant of Q.
	   -1: det(Q) = -1,
	    0: no constraint, or
	    1: det(Q) = +1.

	Returns
	-------

	A random uniformly (Haar) distributed orthogonal 
	(n x n) matrix Q.
	*/
	template <
		typename Real,
		typename... ArgumentSet>
	Matrix<Real> randomOrthogonal(
		integer n, 
		ArgumentSet&&... argumentSet)
	{
		ENSURE_OP(n, >=, 0);

		integer orientation = 
			PASTEL_ARG_S(orientation, (integer)0);

		// "How to Generate Random Matrices
		// from the Classical Compact Groups",
		// Francesco Mezzadri,
		// Notices of the AMS,
		// Volume 54, Number 5, 2007.

		Matrix<Real> Q;
		if (n == 0)
		{
			return Q;
		}

		// Generate a random matrix M where each 
		// element is normally distributed.
		Matrix<Real> M = randomGaussianMatrix<Real>(n, n);

		// Take the qr-decomposition of M.
		Eigen::ColPivHouseholderQR<Matrix<Real>> qr(M);
		Q = qr.matrixQ();
		const auto& R = qr.matrixR();

		// The Q is now a random orthogonal matrix, but it
		// is not quite uniformly (Haar) distributed. The 
		// problem is that the qr-decomposition algorithm has
		// freedom in how to choose the signs of the diagonal
		// elements of R, which brings in systematic bias.
		// We remove this bias by forcing the diagonal of R 
		// to be non-negative.
		for (integer i = 0; i < n; ++i) {
			if (R(i, i) < 0) {
				Q.col(i) = -Q.col(i);
			}
		}

		// There is no need to negate the corresponding 
		// rows of R, since we do not use R.

		if (orientation != 0 &&
			sign(determinant(Q)) != sign(orientation))
		{
			// The sign of the determinant is incorrect.
			// Negate some column of Q. 
	        Q.col(0) = -Q.col(0);

	        // This negation does not affect the uniform
	        // distribution property, because choosing to
	        // do it is based on the properties of the 
	        // generated random matrix.
		}

		return Q;
	}

}

#endif
