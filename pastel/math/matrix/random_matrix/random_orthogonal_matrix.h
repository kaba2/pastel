// Description: Uniform random orthogonal matrix

#ifndef PASTELMATH_RANDOM_ORTHOGONAL_MATRIX_H
#define PASTELMATH_RANDOM_ORTHOGONAL_MATRIX_H

#include "pastel/math/matrix/matrix.h"
#include "pastel/sys/random/random_gaussian.h"
#include "pastel/sys/math/sign.h"
#include "pastel/math/matrix/matrix_decompositions.h"

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Returns a uniformly distributed random orthogonal matrix.
	/*!
	Preconditions:
	n >= 0

	The returned matrix is Q in RR^{n x n} such that
	Q^T Q = I. If orientation != 0, then also 
	det(Q) = sign(orientation).

	n:
	The dimension of the matrix; Q in R^{n x n}.

	orientation:
	If zero, then det(Q) = +-1.
	If negative, then det(Q) = -1.
	If positive, then det(Q) = +1.
	*/
	template <typename Real>
	Matrix<Real> randomOrthogonal(
		integer n, integer orientation = 0)
	{
		ENSURE_OP(n, >=, 0);

		// "How to Generate Random Matrices
		// from the Classical Compact Groups",
		// Francesco Mezzadri,
		// Notices of the AMS,
		// Volume 54, Number 5, 2007.

		Matrix<Real> x(n, n);

		for (auto& value : view(x).range())
		{
			value = randomGaussian<Real>();
		}

		auto qr = x.colPivHouseholderQr();
		Matrix<Real> q = qr.householderQ();
		decltype(auto) r = qr.matrixR();

		for (integer i = 0;i < n;++i)
		{
			if (r(i, i)  < 0)
			{
				q.col(i) = -q.col(i);
			}
		}

		if (orientation != 0)
		{
			if (sign(q.determinant()) != sign(orientation))
			{
				// While the following gives the correct
				// determinant, and preserves orthogonality,
				// I don't have a proof that this will preserve
				// the uniform distribution property. This may
				// or may not be correct.
				for (integer i = 0;i < n;++i)
				{
					q(i, 0) = -q(i, 0);
				}
			}
		}

		return q;
	}

}

#endif
