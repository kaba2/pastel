// Description: Uniform random rotation
// Documentation: random_rotation_matrix.txt

#ifndef PASTELMATH_RANDOM_ORTHOGONAL_H
#define PASTELMATH_RANDOM_ORTHOGONAL_H

#include <armadillo>

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
	arma::Mat<Real> randomOrthogonal(
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

		// Generate a random matrix M where each 
		// element is normally distributed.
		arma::Mat<Real> M = arma::randn<arma::Mat<Real>>(n, n);

		// Take the qr-decomposition of M.
		arma::Mat<Real> Q, R;
		arma::qr(Q, R, M);

		// The Q is now a random orthogonal matrix, but it
		// is not quite uniformly (Haar) distributed. The 
		// problem is that the qr-decomposition algorithm has
		// freedom in how to choose the signs of the diagonal
		// elements of R, which brings in systematic bias.
		// We remove this bias by forcing the diagonal of R 
		// to be non-negative.
		Q.cols(R.diag() < 0) = -Q.cols(R.diag() < 0);

		// There is no need to negate the corresponding 
		// rows of R, since we do not use R.

		if (orientation != 0 &&
			sign(arma::det(Q)) != sign(orientation))
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
