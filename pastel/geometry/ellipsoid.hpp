#ifndef PASTEL_ELLIPSOID_HPP
#define PASTEL_ELLIPSOID_HPP

#include "pastel/geometry/ellipsoid.h"

#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Matrix<Real, N, N> ellipsoidQuadraticForm(
		const Matrix<Real, N, N>& basis)
	{
		// A origin-centered ellipsoid Q is given by the set:
		// Q = {p | f(p) = 1}
		// where
		// f(p) = p^T S p
		// p is in R^(n x 1)
		// S is in R^(n x n) and symmetric positive semi-definite
		//
		// We would like to transform Q by a linear transformation L.
		// Let us try to find how this affects the coefficient matrix:
		// L(Q) = {Lp | f(p) = 1}
		//      = {p | f(L^-1 p) = 1}
		//      = {p | p^T L^-T S L^-1 p = 1}
		//      = {p | p^T S' p = 1}
		// where
		// S' = L^-T S L^-1
		//
		// Thus we see that to transform an ellipsoid by a linear
		// transformation we just need to multiply its
		// coefficient matrix with proper matrices from both sides.
		//
		// Clearly L^-T S L^-1 is symmetric:
		// (L^-T S L^-1)^T = L^-T S^T L^-1 = L^-T S L^-1
		// To see it is positive semi-definite, let
		// the spectral decomposition of S = M^T A M. Now:
		// (for all x:)
		// x^T L^-T S L^-1 x
		// = x^T L^-T M^T A M L^-1 x
		// = x^T L^-T M^T sqrt(A^T) sqrt(A) M L^-1 x
		// = (sqrt(A) M L^-1 x)^T (sqrt(A) M L^-1 x)
		// = |sqrt(A) M L^-1 x|^2 >= 0
		//
		// Thus the image of an ellipsoid under a linear transformation
		// is an ellipsoid.
		//
		// The unit sphere is given by S = I. Thus if we are given
		// a linear transformation of this sphere then
		// the resulting quadric has S' = L^-T L^-1.

		// Of course, we work with row vectors
		// and thus the correct matrix to return is
		// ((L^T)^-T (L^T)^-1)^T = L^-1 L^-T.

		Matrix<Real, N, N> result = inverse(basis);
		result *= transpose(result);
		
		return result;
	}

	template <int N, typename Real>
	AlignedBox<Real, N> ellipsoidBoundingAlignedBox(
		const Matrix<Real, N, N>& quadraticForm)
	{
		// TODO: What if the 'quadraticForm'
		// is not invertible?

		// I credit 'Dave' from sci.math for
		// the solution of this problem. Discussions
		// with Dave Eberly from comp.graphics.algorithms
		// were also helpful.
		//
		// Let
		// f(x) = x^T S x
		// S symmetric and positive semi-definite
		//
		// Note that:
		// gradient(f)(x) = 2Sx
		//
		// We wish to compute the bounds of the ellipsoid
		// along a unit vector n. To do this, we use
		// Lagrange multipliers to solve a constrained
		// maximization problem.
		//
		// We wish to maximize the length of the
		// projection to n:
		//
		// f(x) = n^T x
		//
		// with the constraint that the vector
		// lies on the ellipsoid:
		//
		// x^T S x = 1
		//
		// To solve this, we transform the problem
		// to an unconstrained problem of maximizing:
		//
		// h(x, t) = n^T x + t (x^T S x - 1)
		//
		// The gradient of h is given by:
		// g(x, t) = n + 2t Sx
		//
		// The derivative of h w.r.t. t is:
		// (dh/dt)(x, t) = x^T S x - 1
		//
		// We set these derivatives to zero to
		// find the maximum:
		//
		// n + 2t Sx = 0    (1)
		// x^T S x - 1 = 0  (2)
		//
		// (1)
		// => x = -S^-1 n / 2t
		//
		// (1) => (2)
		// (1 / (4t^2)) n^T S^-T S S^-1 n - 1
		// = (1 / (4t^2)) n^T S^-T n - 1 = 0
		// =>
		// n^T S^-T n / 4 = t^2
		// =>
		// n^T S^-1 n / 4 = t^2
		// =>
		// t = +/- sqrt(n^T S^-1 n) / 2
		//
		// (2) => (1)
		// x = +/- S^-1 n / sqrt(n^T S^-1 n)
		//
		// We are only interested in the length along
		// the vector n:
		//
		// n^T x = +/- n^T S^-1 n / sqrt(n^T S^-1 n)
		// = +/- sqrt(n^T S^-1 n)
		//
		// We shall now use this formula for the standard
		// basis axes. In this case:
		//
		// e_i^T x = +/- sqrt(e_i^T S^-1 e_i)
		// = +/- sqrt(S^-1(i, i))

		const Matrix<Real, N, N> invQuadraticForm =
			inverse(quadraticForm);

		// Actually, being positive semi-definite
		// implies that the diagonal elements
		// are non-negative. We are just being
		// careful for rounding errors
		// here with the mabs().

		const Vector<Real, N> radius = sqrt(mabs(
			diagonal(invQuadraticForm)));

		const AlignedBox<Real, N> bound(-radius, radius);

		return bound;
	}

}

#endif
