#ifndef PASTELMATH_AFFINE_LEAST_SQUARES_HPP
#define PASTELMATH_AFFINE_LEAST_SQUARES_HPP

#include "pastel/math/affine_least_squares.h"

namespace Pastel
{

	template <typename Real>
	AffineTransformation<Real> affineLeastSquares(
		const Matrix<Real>& from,
		const Matrix<Real>& to)
	{
		ENSURE_OP(from.m(), ==, to.m());
		ENSURE_OP(from.n(), ==, to.n());

		// Let P = [p_1, ..., p_n] in R^{m x n}
		// and R = [r_1, ..., r_n] in R^{m x n}
		// represent sets of points in R^m.
		//
		// f : R^m -> R^m : f(x) = Ax + b
		//
		// The affine transformation that maps P to R, while
		// minimizing least-squares error is given by:
		//
		// A = (P' R'^T)(P' P'^T)^-1
		// b = r' - Ap'
		//
		// where
		// p' = (1 / n) sum_{i = 1}^n p_i
		// r' = (1 / n) sum_{i = 1}^n r_i
		// P' = [p_1 - p', ..., p_n - p']
		// R' = [r_1 - r', ..., r_n - r']

		integer m = from.m();
		integer n = from.n();

		if (n == 0 || m == 0)
		{
			// No points to match, or zero dimension.
			// Return the identity transformation in R^m.
			return AffineTransformation<Real>(m);
		}

		if (n == 1)
		{
			// The P' P'^T is always singular when n = 1.
			// We will make the transformation an exact
			// translation.
			return AffineTransformation<Real>(
				identityMatrix<Real>(m, m),
				to.column(0) - from.column(0));
		}

		// Compute centroids
		Vector<Real, N> fromCentroid =
			sum(transpose(from)) / n;
		Vector<Real, N> toCentroid =
			sum(transpose(to)) / n;

		// Compute PR^T and PP^T.
		Matrix<Real> prt(m, m);
		Matrix<Real> ppt(m, m);
		for (integer i = 0;i < n;++i)
		{
			ppt += outerProduct(from.column(i) - fromCentroid);

			// PR^T = [p_1, ..., p_n] [r_1, ..., r_n]^T
			//      = [p_1, ..., p_n] [r_1^T; ...; r_n^T]
			//      = sum_{i = 1}^n p_i r_i^T
			prt += outerProduct(
				from.column(i) - fromCentroid, 
				to.column(i) - toCentroid);
		}

		// Compute A = (P' R'^T)(P' P'^T)^-1

		Matrix<Real> a = prt * inverse(ppt);

		// Compute b = r' - Ap'.
		return AffineTransformation<Real>(
			std::move(a), toCentroid - a * fromCentroid);
	}

}

#endif
