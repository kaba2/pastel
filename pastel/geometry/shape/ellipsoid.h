// Description: Algorithms for ellipsoids

#ifndef PASTELGEOMETRY_ELLIPSOID_H
#define PASTELGEOMETRY_ELLIPSOID_H

#include "pastel/math/matrix/matrix.h"
#include "pastel/math/matrix/matrix_inverse.h"

namespace Pastel
{

	//! Computes quadratic form coefficients for an ellipse.
	/*!
	An origin-centered ellipsoid Q is be given by:
	Q = {p | f(p) = 1}
	where
	f(p) = p S p^T
	p is in R^(1 x n)
	S is in R^(n x n) and symmetric positive semi-definite

	This function returns a matrix S such that
	the set Q is an image of the unit sphere under
	the given linear transformation.
	*/
	template <typename Real, int M, int N>
	Matrix<Real, M, N> ellipsoidQuadraticForm(
		const Matrix<Real, M, N>& basis)
	{
		// An origin-centered ellipsoid Q is given by the set:
		// Q = {p | f(p) = 1}
		// where
		// f(p) = p^T S p
		// S is in R^(n x n) and symmetric positive semi-definite
		// p is in R^n
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
		// the resulting quadric has S' = L^-T L^-1 = (L L^T)^-1.

		return inverse(basis * transpose(basis));
	}

	//! Returns a minimum volume aligned box bounding the given ellipse.
	template <typename Real, int M, int N>
	AlignedBox<Real, N> ellipsoidBoundingAlignedBox(
		const Matrix<Real, M, N>& quadraticForm)
	{
		// TODO: What if the 'quadraticForm'
		// is not invertible?

		// I credit 'Dave' from sci.math for
		// the solution of this problem. Discussions
		// with Dave Eberly in comp.graphics.algorithms
		// were also helpful.
		//
		// Let
		// p : R^n -> R: p(x) = x^T S x
		// S symmetric and positive semi-definite
		//
		// Note that:
		// gradient(p)(x) = 2Sx
		//
		// We wish to compute the bounds of the ellipsoid
		// E = {x in R^n : p(x) = 1}
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

		Vector<Real, N> radius = 
			sqrt(diagonal(inverse(quadraticForm)));

		return AlignedBox<Real, N>(-radius, radius);
	}

}

#endif
