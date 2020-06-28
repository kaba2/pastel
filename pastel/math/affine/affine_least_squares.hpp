#ifndef PASTELMATH_AFFINE_LEAST_SQUARES_HPP
#define PASTELMATH_AFFINE_LEAST_SQUARES_HPP

#include "pastel/math/affine/affine_least_squares.h"

namespace Pastel
{

	//! Least-squares affine transformation between paired point-sets.
	/*!
	returns:
	The affine transformation with minimal least-squares error,
	if PP^T is non-singular, and SingularMatrix_Exceptiocols() 
	otherwise. As a special case, the n = 1 case is returned
	as an exact translation (although here PP^T is always
	singular).
	*/
	template <typename Real, int M, int N>
	AffineTransformation<Real, M, N> affineLeastSquares(
		const Matrix<Real, M, N>& from,
		const Matrix<Real, M, N>& to)
	{
		ENSURE_OP(from.rows(), ==, to.rows());
		ENSURE_OP(from.cols(), ==, to.cols());

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

		integer m = from.rows();
		integer n = from.cols();

		if (n == 0 || m == 0)
		{
			// No points to match, or zero dimension.
			// Return the identity transformation in R^m.
			return AffineTransformation<Real, M, N>(m);
		}

		if (n == 1)
		{
			// The P' P'^T is always singular when n = 1.
			// We will make the transformation an exact
			// translation.
			return AffineTransformation<Real, M, N>(
				identityMatrix<Real, M, N>(m, m),
				to.col(0) - from.col(0));
		}

		// Compute centroids
		Vector<Real, M> fromCentroid =
			sum(transpose(from)) / n;
		Vector<Real, M> toCentroid =
			sum(transpose(to)) / n;

		// Compute PR^T and PP^T.
		Matrix<Real, M, M> prt = Matrix<Real, M, M>::Zero(m, m);
		Matrix<Real, M, M> ppt = Matrix<Real, M, M>::Zero(m, m);
		for (integer i = 0;i < n;++i)
		{
			ppt += outerProduct(from.col(i) - fromCentroid);

			// PR^T = [p_1, ..., p_n] [r_1, ..., r_n]^T
			//      = [p_1, ..., p_n] [r_1^T; ...; r_n^T]
			//      = sum_{i = 1}^n p_i r_i^T
			prt += outerProduct(
				from.col(i) - fromCentroid, 
				to.col(i) - toCentroid);
		}

		// Compute A = (P' R'^T)(P' P'^T)^-1

		Matrix<Real, M, M> a = prt * inverse(ppt);

		// Compute b = r' - Ap'.
		return AffineTransformation<Real, M>(
			std::move(a), toCentroid - a * fromCentroid);
	}

}

#endif
